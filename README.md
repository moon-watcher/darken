# Darken — DARKula ENgine Entity System

`darken.h` is a generic, single-header entity/lifecycle manager for C — target-agnostic, with no dependency on any specific platform, engine, or toolchain. It operates purely over plain caller-provided storage and pointers, so it drops into any C project. No dynamic dispatch, no hidden allocations after setup, no handle tables — entities are plain pointers into a caller-provided storage block, kept dense by swap-and-pop.

Current version: **darken-1.3.0_dev**

- [Darken — DARKula ENgine Entity System](#darken--darkula-engine-entity-system)
  - [Requirements](#requirements)
  - [Core idea](#core-idea)
  - [The two zones](#the-two-zones)
  - [Two guarantees you can rely on](#two-guarantees-you-can-rely-on)
  - [Getting a `darken_t` — three ways](#getting-a-darken_t--three-ways)
  - [Spawning an entity](#spawning-an-entity)
  - [Update / lifecycle control — two modes](#update--lifecycle-control--two-modes)
    - [STATE-MACHINE mode (default)](#state-machine-mode-default)
    - [DIRECT mode](#direct-mode)
  - [Delete, migrate, swap](#delete-migrate-swap)
  - [Iterating](#iterating)
  - [API reference](#api-reference)
  - [Gotchas](#gotchas)

## Requirements

Three things are required of whoever includes `darken.h`:

1. **Fixed-width integer types visible before including the header.** Darken deliberately does **not** `#include <stdint.h>` itself — the including project must provide `uint8_t`, `uint16_t`, `uint32_t`, and `uintptr_t`, whether via a plain `#include <stdint.h>` or whatever equivalent your target already provides them through.

2. **A GNU C compiler — GCC or Clang.** Darken relies on GNU extensions such as `__attribute__((aligned))` and `__alignof__` in the storage declarations and stride calculation, so it will not build under a strict ISO-C-only compiler. Sentinel handling in state-machine mode (`==` against `DARKEN_CONTINUE`, `>` against `DARKEN_CONTINUE` to detect a new callback) additionally relies on GNU C / target-ABI behavior for converting small integer values to function pointers and comparing function-pointer values with those sentinels.

3. **A non-zero capacity and a stride that fits in `uint16_t`.** `CAPACITY` must be greater than zero, and the computed entity stride must fit in `uint16_t`. A single `darken_t` therefore tops out at 65535 entities, while the payload size must also be small enough that the complete aligned entity stride remains representable in `uint16_t`.

Beyond that, Darken makes no assumptions about the target: pointer width, struct alignment, and endianness are all whatever the compiler says they are for the platform it's building for — entity storage alignment is computed with `__alignof__` rather than any hardcoded value.

The payload type used with `DARKEN_DATA()` must not require stricter alignment than `struct darken_entity_t` itself. Darken knows the payload size when storage is declared, but not the payload type's required alignment.

## Core idea

A `darken_t` (the **ctx**) owns an array of pointers (`pool[]`) into a fixed block of entity storage you provide. Each entity is a `struct darken_entity_t` with a flexible array member `data[]` for your own payload — so one allocation holds the entity's bookkeeping (`slot`, `update`, `destroy`, `tag`, `usr`, `owner`) and your game-specific fields back to back.

The ctx keeps `pool[]` partitioned into two contiguous zones and moves entities between them purely by swapping pointers around — an entity's own address never changes, only its *position* in `pool[]` does. `darken_entity_swap()` and `darken_entity_migrate()` extend this same idea across **two different** ctx's, not just within one.

## The two zones

```text
[ active entities ][   free slots    ]
0                  size               capacity
```

| Zone   | Range              | Meaning                                                                                 |
| ------ | ------------------ | --------------------------------------------------------------------------------------- |
| Active | `[0, size)`        | Updated every frame by `darken_update()`. Visited by `DARKEN_FOREACH` in reverse order. |
| Free   | `[size, capacity)` | Unused slots. `DARKEN_SPAWN()` always takes from here.                                  |

There is no built-in "paused" zone. If your game needs a dormant/inactive set that isn't touched by `darken_update()`, keep a second, smaller `darken_t` for it and move entities in and out with `darken_entity_migrate()` — see [Delete, migrate, swap](#delete-migrate-swap).

## Two guarantees you can rely on

1. **An entity's address never moves** once `darken_init()` has run. Spawning, deleting, migrating, or swapping entities only ever reorders `pool[]`; it never relocates the storage itself. That's what makes it safe to keep a raw pointer into `entity->data` across frames.

2. **The ctx's own address must be stable too**, for the same reason: `darken_init()` bakes the address you gave it into every entity's `->owner`. A helper that builds a `darken_t` on its own stack and returns it *by value* compiles fine and silently leaves every `->owner` dangling into a dead stack frame the moment the helper returns.

```c
// WRONG — m's address dies with the function
darken_t make_pool(void)
{
    darken_t m = DARKEN_ALLOC(malloc, 8, sizeof(Enemy));
    darken_init(&m);
    return m;
}

// RIGHT — build it in its final home
void make_pool(darken_t *m)
{
    *m = DARKEN_ALLOC(malloc, 8, sizeof(Enemy));
    darken_init(m);
}
```

Neither `darken_init()`, `DARKEN_SPAWN()`, nor deletion clears `update`/`destroy`/`tag`/`usr` — an entity with a recycled slot may retain values left by the previous occupant. Set every field you care about on every spawn.

## Getting a `darken_t` — three ways

```c
// Dynamic allocation
darken_t m = DARKEN_ALLOC(malloc, 5, sizeof(struct MyComponent));
darken_init(&m);
...
darken_reset(&m);
DARKEN_FREE(free, &m);

// Static storage, bound at runtime
DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
darken_t m = DARKEN_BIND(storage);
darken_init(&m);

// Static storage, bound as a compile-time constant
DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
static darken_t m = DARKEN_INIT(storage);
```

All three (`DARKEN_ALLOC`, `DARKEN_BIND`, `DARKEN_INIT`) expand to a `(darken_t){...}` compound literal, so they work anywhere a `darken_t` expression is legal — a declaration initializer, a plain assignment to reassign an existing variable, a function argument, and so on.

`DARKEN_INIT` is intended for static/file-scope storage where the storage object itself has static storage duration.

`DARKEN_ALLOC()` takes an allocator compatible with the size argument it receives and returning a pointer to the allocated block.

`DARKEN_ALLOC()` does not handle allocation failure or partial allocation cleanup; validate `pool` and `storage` before calling `darken_init()`.

## Spawning an entity

```c
darken_entity_t e = DARKEN_SPAWN(&m);

if (!e) return;

e->update = enemy_walk_state;
e->destroy = enemy_on_death;
e->tag = ENEMY;
e->usr = WHATEVER;

DARKEN_DATA(struct MyComponent, data, e);

data->hp = 10;
```

A freshly spawned entity may be a recycled storage slot. Initialize every field your entity actually needs, including clearing anything you do not want inherited from the previous occupant.

## Update / lifecycle control — two modes

**STATE-MACHINE mode is the default.** Define `DARKEN_DIRECT` before including this header to opt into direct mode instead. Pick one per translation unit — it's a compile-time switch on `darken_state_t` itself.

### STATE-MACHINE mode (default)

```c
void *player_walk(struct Player *p)
{
    p->x++;

    if (should_stop(p))
        return player_stop;   // installs a new callback for NEXT frame

    if (should_die(p))
        return DARKEN_DELETE; // destroy() runs (if set), then the entity dies

    return DARKEN_CONTINUE;   // stay active, same callback
}
```

`darken_update()` reads the return value and drives the lifecycle for you:

| Return            | Effect                                                       |
| ----------------- | ------------------------------------------------------------ |
| `DARKEN_CONTINUE` | Stay active, keep the same `update`                          |
| `DARKEN_DELETE`   | Call `destroy` (if set), then delete the entity              |
| anything else     | Treated as a new `update` callback, installed for next frame |

The callback only ever receives the payload, never the entity handle — recover it with `DARKEN_ENTITY(data)` if you need to touch `usr`/`tag`.

```c
void *player_walk(struct Player *p) { ... }
```

— assigns straight to `entity->update` with no cast. Only declare `void *data` for callbacks that genuinely ignore the payload.

`destroy` uses the same `darken_state_t` callback type and the same `(data)` argument convention as `update`. Its return value is always ignored — `darken_reset()` and `darken_entity_delete()` only ever call it for its side effects.

`destroy` must not mutate the ctx's pool zones. Deleting, spawning, migrating, or swapping entities from inside a `destroy` callback will corrupt the swap state and iteration that the engine relies on. This applies to every path that invokes `destroy`: `darken_reset()`, `darken_entity_delete()`, and the `DARKEN_DELETE` branch inside `darken_update()`.

### DIRECT mode

```c
#define DARKEN_DIRECT
#include "darken.h"

void player_stop_state(darken_entity_t entity, struct Player *p)
{
    if (should_walk(p))
        entity->update = player_walk_state;
}
```

Both the handle and the payload are passed; `darken_update()` calls `entity->update(entity, entity->data)` and ignores any return value. The callback is in full control: change state by assigning `entity->update` directly, and delete itself by calling `darken_entity_delete()` with the handle it was already given — no `DARKEN_ENTITY` lookup needed. `darken_entity_migrate()` and `darken_entity_swap()` are available too, and take the entity handle directly for the same reason.

The `destroy` callback uses the same `darken_state_t` callback type and the same `(entity, data)` argument convention as `update`; its return value is ignored.

A callback that only declares the entity parameter can drop the second parameter: `void player_walk(darken_entity_t entity)`.

```c
#define DARKEN_DIRECT
#include "darken.h"

void player_walk_state(darken_entity_t entity)
{
    DARKEN_DATA(struct Player, p, entity);

    p->x++;

    if (should_stop(p))
        entity->update = player_stop_state;

    if (should_die(p))
        entity->update = player_destroy_state;
}

void player_destroy_state(darken_entity_t entity)
{
    darken_entity_delete(entity);
}
```

The old-style, unprototyped callback type is a GNU-C/ABI-oriented convenience. It is not a general ISO-C guarantee that arbitrary incompatible function signatures may be interchanged safely.

## Delete, migrate, swap

There is no pause/resume in Darken. These three primitives cover deleting an entity, moving it into a *different* `darken_t`, and swapping two entities' positions — including across two different ctx's.

```c
darken_entity_delete(e);

// destroy() runs (if set, and if e was active), then e's slot returns to free
// in its own ctx. No-op if e is already free.

darken_entity_t moved = darken_entity_migrate(e, &other_pool);

// Copies min(e's stride, other_pool's stride) bytes of e's whole entity
// (header + payload) into a free slot in other_pool, WITHOUT calling
// destroy(). If e was active, it becomes free in its original pool; if e
// was already free, its original pool is untouched but other_pool still
// gains a new active entity built from e's (already-recycled) bytes -- see
// Gotchas. Returns the entity as it now lives in other_pool, or 0 if
// other_pool is full or is e's own pool.

darken_entity_swap(a, b);

// a and b trade places: pool slot, owner, and active/free status all swap.
// Works even when a and b belong to different darken_t's. Payload data is
// never touched -- only the bookkeeping (slot/owner/pool entries) moves.
```

`darken_entity_migrate()` is the tool for a "paused set": keep a second, smaller `darken_t` (it can even use a more compact payload struct than the active one — see Gotchas for what a stride mismatch does) that you simply never pass to `darken_update()`, and migrate entities into and out of it as needed.

`darken_entity_swap()` is lower-level still: it never touches `size` or payload data in either ctx, so it's the right tool when you want to trade two entities' positions directly rather than copy anything. Both entities may belong to the same ctx or to different ctx's.

## Iterating

```c
DARKEN_FOREACH(&m,
{
    DARKEN_DATA(struct MyComponent, data, _entity);
    data->hp -= 1;
});
```

Visits active entities in **reverse** slot order (`size-1` down to `0`), which makes deleting the **currently visited** entity safe.

One subtlety: if deleting entity A swaps a *different*, not-yet-visited entity B into A's old (already-passed) slot, B is skipped for *this* pass and picked up again next frame — not a bug, just a one-frame ordering quirk of swap-and-pop under reverse iteration.

More generally, deleting, migrating, or swapping other entities from inside the iteration body can affect which entities are visited during that pass.

The `ctx` argument is evaluated more than once by `DARKEN_FOREACH`, so pass a stable expression such as `&m`, not an expression with side effects.

## API reference

| Symbol                                         | What it does                                                                       |
| ---------------------------------------------- | ---------------------------------------------------------------------------------- |
| `DARKEN_ALLOC(alloc, capacity, payload_size)`  | Heap-backed `darken_t`                                                             |
| `DARKEN_FREE(free, ctx)`                       | Frees what `DARKEN_ALLOC` allocated; does not call `destroy()`                     |
| `DARKEN_DECLARE(name, capacity, payload_size)` | Declares static storage (no `darken_t` yet)                                        |
| `DARKEN_BIND(name)`                            | `darken_t` view over `DARKEN_DECLARE`d storage, for runtime (re)binding            |
| `DARKEN_INIT(name)`                            | Same, as a static-storage initializer                                              |
| `darken_init(ctx)`                             | Lays entities out over the storage block and resets the logical population to zero |
| `DARKEN_SPAWN(ctx)`                            | Takes a slot from the free zone, or `0` if full                                    |
| `DARKEN_DATA(type, var, entity)`               | Declares `type *var` pointing at `entity`'s payload                                |
| `DARKEN_ENTITY(data)`                          | Recovers the `darken_entity_t` from a payload pointer                              |
| `darken_entity_delete(entity)`                 | Destroys (if active) and frees an entity                                           |
| `darken_entity_migrate(entity, dst)`           | Copies an entity into another `darken_t`; see above                                |
| `darken_entity_swap(e1, e2)`                   | Trades two entities' positions, even across two different ctx's                    |
| `darken_update(ctx)`                           | Runs one frame over the active zone                                                |
| `darken_reset(ctx)`                            | Destroys all active entities, then empties the pool                                |
| `DARKEN_FOREACH(ctx, code)`                    | Manual iteration over the active zone                                              |
| `DARKEN_ENTITY_IS_ACTIVE/FREE(entity)`         | Zone membership tests                                                              |
| `DARKEN_COUNT_ACTIVE/FREE(ctx)`                | Zone sizes                                                                         |

## Gotchas

- `capacity`/`size`/`stride` are `uint16_t`, so a single `darken_t` tops out at 65535 entities, and the computed entity stride must also fit in `uint16_t`.

- **`DARKEN_SPAWN`/`DARKEN_FOREACH` evaluate their `ctx` argument more than once.** `DARKEN_SPAWN(managers[idx++])` contains multiple unsequenced evaluations of `idx++` in one expression and is undefined behavior in C. `DARKEN_FOREACH(managers[idx++], ...)` evaluates its `ctx` argument twice in separate statements, so it is not the same undefined-behavior case, but `idx` still advances twice. Only ever pass a stable expression such as `&m` or a plain pointer variable to either macro.

- **`darken_entity_migrate()` is not a no-op on a free entity.** If `entity` is already free, its source ctx is left untouched, but `dst` still gains a new active entity built from `entity`'s (already-recycled, meaningless) payload bytes. Calling it by mistake on something you thought was already gone silently spends a slot in `dst`, it does not return early.

- **`darken_entity_migrate()` truncates or leaves stale data on a stride mismatch.** Exactly `min(src->stride, dst->stride)` bytes are copied. Moving into a smaller-stride ctx truncates the tail of the payload; moving into a larger-stride one leaves the payload's tail as whatever was already in that recycled slot. The header fields (`slot`, `usr`, `update`, `destroy`, `tag`, `owner`) are copied first and then `slot`/`owner` are corrected for `dst`. If you rely on specific payload fields surviving a round trip through a smaller ctx, put those fields first in both payload structs.

- **No staleness detection.** Darken hands you raw pointers, not generation-checked handles. If you keep a `darken_entity_t` around after deleting it, and a later spawn reuses that exact slot, your old pointer now silently refers to the *new* occupant — same address, different entity. If you need to detect this, roll your own generation counter in `tag` or `usr` and check it yourself; Darken has no built-in way to ask "is this still the entity I think it is?".

- Fields aren't auto-initialized on spawn (see above) — an entity with a stale or invalid `update` pointer will crash the moment `darken_update()` reaches it.

- **Payload alignment is a precondition.** The payload type used with `DARKEN_DATA()` must not require stricter alignment than `struct darken_entity_t`. The storage helper knows the payload size, not its C type or alignment. `darken_entity_migrate()`'s internal copy leans on this same guarantee to move data in 32-bit words: it's safe without a runtime check because every entity address is a multiple of `__alignof__(struct darken_entity_t)`, and that value already accounts for `uint32_t` alignment on whatever target you're building for.

- `darken_init()` resets the logical pool without calling `destroy()`. Do not call it on a live population unless that population is intentionally being discarded.

- `DARKEN_FREE()` releases storage only; it does not run `destroy()` callbacks. Call `darken_reset()` first when destructor side effects are required.

- `DARKEN_ALLOC()` does not handle allocation failure or partial allocation cleanup. Check its returned pointers before calling `darken_init()`.

- Toolchain and integer-type constraints (GNU C, `<stdint.h>`) are covered in [Requirements](#requirements), not repeated here.
