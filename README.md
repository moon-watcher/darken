# Darken — DARKula ENgine Entity System

`darken.h` is a generic, single-header entity/lifecycle manager for C — target-agnostic, with no dependency on any specific platform, engine, or toolchain. It operates purely over plain caller-provided storage and pointers, so it drops into any C project, from an 8-bit home computer to a 64-bit workstation. No dynamic dispatch, no hidden allocations after setup, no handle tables — entities are plain pointers into a caller-provided storage block, kept dense by swap-and-pop.

Current version: **darken-1.4.0_dev**

- [Darken — DARKula ENgine Entity System](#darken--darkula-engine-entity-system)
  - [Requirements](#requirements)
  - [Configuration](#configuration)
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

`darken.h` has **zero `#include` lines of its own**. Everything in it is built from bare C keywords, macros, and two small self-contained tricks for offsets and alignment — no `<stddef.h>`, no `offsetof()`, no compiler-specific `__alignof__` or `__attribute__`. What's required of whoever includes it:

1. **Fixed-width integer types visible before including the header.** Darken deliberately does **not** `#include <stdint.h>` itself — the including project must provide `uint8_t`, `uint16_t`, and `uint32_t` (whichever ones the configuration you pick actually needs — see [Configuration](#configuration)), whether via a plain `#include <stdint.h>` or whatever equivalent your target already provides them through.

2. **Standard C89, plus one extension: C99 flexible array members** (the trailing `data[]` payload member). That's it — no GNU-only attributes, no C11 features. Flexible array members have been part of ISO C since 1999, and were already a common pre-standard extension before that; both `cc65` (6502, e.g. the C64) and `SDCC` (Z80, e.g. the Master System) support them. `inline` isn't available in strict C89 either, so every function is declared through `DARKEN_INLINE`, which becomes `static inline` on a C99-or-later compiler and plain `static` otherwise — predefine `DARKEN_INLINE` yourself to override.

   `DARKEN_ALLOC()`, `DARKEN_BIND()`, and `DARKEN_INIT()` are written as C99 compound literals for convenience. If your compiler is C89-only and lacks those, nothing about the engine itself depends on them: replace `darken_t m = DARKEN_ALLOC(...);` with a zero-initialized `darken_t m = {0};` followed by assigning `m.pool`, `m.storage`, `m.capacity`, `m.stride` by hand.

3. **`CAPACITY` must satisfy `1 <= CAPACITY <= max(darken_index_t)`, and the computed entity stride must fit in `darken_index_t` too.** `darken_index_t` defaults to `uint16_t` (65535 entities), or `uint8_t` (255 entities) under the `DARKEN_SMALL` preset. `DARKEN_DECLARE()` rejects `CAPACITY == 0` at compile time for free (as a side effect of how its storage array is sized), but can't check the upper bound without a second declaration, which would break `static DARKEN_DECLARE(...)`. `DARKEN_ALLOC()` checks neither bound, since its `CAPACITY` is an ordinary runtime value. Exceeding either limit silently truncates the stored capacity/stride at runtime rather than failing to compile.

Beyond that, Darken makes no assumptions about the target: pointer width, struct alignment, and endianness are all whatever the compiler says they are for the platform it's building for. Alignment is derived from `sizeof()` arithmetic and a null-pointer offset idiom, never a hardcoded value or a compiler builtin.

**One genuinely non-portable corner, and it's opt-in:** state-machine mode's sentinel trick round-trips small integers through a function-pointer type and compares them with `>`. That's well-defined in practice on any architecture with a single flat code/data address space — i.e. essentially every CPU anyone targets today (6502, Z80, 68k, x86, ARM, RISC-V, MIPS...). It is *not* guaranteed by the C standard, and is *not* safe on genuine Harvard-architecture micros where code and data live in separate address spaces (classic AVR, some PIC cores). If that's your target, define `DARKEN_DIRECT` (see below) — direct mode never converts an integer to a function pointer, so the concern disappears entirely.

The payload type used with `DARKEN_DATA()` must not require stricter alignment than `struct darken_entity_t` itself.

## Configuration

Every knob below is optional, opt-in, and defaulted for 32/64-bit targets. Nothing needs to be defined to use Darken as-is; the knobs exist for 8-bit and 16-bit targets where memory is the scarce resource.

| Macro                   | Default         | Effect                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| ----------------------- | --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `DARKEN_SMALL`          | —               | Convenience preset for 8-bit targets. Sets `DARKEN_INDEX_T` and `DARKEN_USR_T` to `uint8_t`, but only for whichever of the two you haven't already defined yourself. Changes types only — removes no field, changes no behaviour.                                                                                                                                                                                                                                                                                                                                        |
| `DARKEN_INDEX_T`        | `uint16_t`      | Type of `slot`, `capacity`, `size`, and `stride`. Switch to `uint8_t` to cap the pool at 255 entities and save a byte or two per header.                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| `DARKEN_USR_T`          | `uint16_t`      | Type of the `usr` field.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| `DARKEN_TAG_T`          | `uint32_t`      | Type of the `tag` field — the single fattest header member on 8-bit targets.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |
| `DARKEN_NO_TAG`         | —               | Removes `tag` entirely. Code touching `entity->tag` fails to compile — that's the intended feedback.                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| `DARKEN_NO_USR`         | —               | Removes `usr` entirely. Same rule as above.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              |
| `DARKEN_NO_DESTROY`     | —               | Removes the `destroy` callback and every destroy-invocation path. Saves one function pointer per entity. Without it, `DARKEN_DELETE` (or a manual `darken_entity_delete()`) just removes the entity — no cleanup hook.                                                                                                                                                                                                                                                                                                                                                   |
| `DARKEN_MIGRATE_WORD_T` | `unsigned char` | Word size for the bulk copy in `darken_entity_migrate()`. The default is `unsigned char` specifically — not `uint8_t` — because it's one of the three types the C strict-aliasing rule names by name; `uint8_t` is almost always the same type under the hood, but the standard doesn't require it to be. Widening this (`uint16_t`/`uint32_t`/`uint64_t`) is a speed knob on hosted 32/64-bit targets, but is then formally UB under strict aliasing (works in practice on compilers that don't exploit it, e.g. SDCC, cc65, or any build with `-fno-strict-aliasing`). |
| `DARKEN_DIRECT`         | —               | Switches from state-machine mode to direct-callback mode. See [Update / lifecycle control](#update--lifecycle-control--two-modes). Also the mode to prefer on Harvard-architecture micros.                                                                                                                                                                                                                                                                                                                                                                               |

`DARKEN_NO_TAG`, `DARKEN_NO_USR`, `DARKEN_NO_DESTROY`, and `DARKEN_DIRECT` are semantic changes and stay explicit opt-ins on purpose — `DARKEN_SMALL` will never silently pull one in.

**Alignment caveat:** `DARKEN_INDEX_T`, `DARKEN_USR_T`, and `DARKEN_TAG_T` all sit in the header before the `data[]` payload, and the whole layout relies on `data[]` landing at an offset that's a multiple of the struct's own alignment. That holds for any integer type up to the pointer's width on every mainstream ABI — except ARM32 EABI, where `uint64_t` is 8-aligned but `void*` is only 4-aligned. Don't set any of the three to `uint64_t` (or another 8-aligned type) there. `uint32_t` and below are always safe, and every preset/default combination already respects this.

## Core idea

A `darken_t` (the **ctx**) owns an array of pointers (`pool[]`) into a fixed block of entity storage you provide. Each entity is a `struct darken_entity_t` with a flexible array member `data[]` for your own payload — so one allocation holds the entity's bookkeeping (`slot`, `update`, and whichever of `destroy`/`tag`/`usr`/`owner` your configuration keeps) and your game-specific fields back to back.

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

Neither `darken_init()`, `DARKEN_SPAWN()`, nor deletion clears `update`/`destroy`/`tag`/`usr` — an entity with a recycled slot may retain values left by the previous occupant. Set every field you care about on every spawn, and **always assign `->update` before the next `darken_update()` call**: there is no NULL check on the update callback (the branch would cost cycles on every entity, every frame), so calling through an unset one is a hard crash on hosted targets and whatever your CPU does on a jump to address 0 on a bare-metal one (on Z80, that's `RST 0` — a machine reset).

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

All three (`DARKEN_ALLOC`, `DARKEN_BIND`, `DARKEN_INIT`) expand to a `(darken_t){...}` compound literal, so they work anywhere a `darken_t` expression is legal — a declaration initializer, a plain assignment to reassign an existing variable, a function argument, and so on. `DARKEN_DECLARE()` itself expands to a single declaration, so it can be prefixed with `static` at file scope too:

```c
static DARKEN_DECLARE(storage, 32, sizeof(struct Bullet));
static darken_t world = DARKEN_INIT(storage);
```

`DARKEN_INIT` is intended for static/file-scope storage where the storage object itself has static storage duration.

`DARKEN_ALLOC()` takes an allocator compatible with the size argument it receives and returning a pointer to the allocated block. It does not handle allocation failure or partial allocation cleanup; validate `pool` and `storage` before calling `darken_init()`. It also trusts the allocator to hand back suitably-aligned memory — true for `malloc()`/`calloc()` on a hosted implementation, not guaranteed for a hand-rolled pool allocator on a freestanding 8-bit target. `DARKEN_DECLARE()`'s static storage is aligned for you regardless.

## Spawning an entity

```c
darken_entity_t e = DARKEN_SPAWN(&m);

if (!e) return;

e->update = enemy_walk_state;
e->destroy = enemy_on_death;   // omit if built with DARKEN_NO_DESTROY
e->tag = ENEMY;                // omit if built with DARKEN_NO_TAG
e->usr = WHATEVER;             // omit if built with DARKEN_NO_USR

DARKEN_DATA(struct MyComponent, data, e);

data->hp = 10;
```

A freshly spawned entity may be a recycled storage slot. Initialize every field your entity actually needs, including clearing anything you do not want inherited from the previous occupant — and assign `->update` before the next frame's `darken_update()`, or see the crash note above.

## Update / lifecycle control — two modes

**STATE-MACHINE mode is the default.** Define `DARKEN_DIRECT` before including this header to opt into direct mode instead. Pick one per translation unit — it's a compile-time switch on `darken_state_t` itself. Direct mode is also the one to reach for on a Harvard-architecture target (see [Requirements](#requirements)).

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

| Return            | Effect                                                               |
| ----------------- | -------------------------------------------------------------------- |
| `DARKEN_CONTINUE` | Stay active, keep the same `update`                                  |
| `DARKEN_DELETE`   | Call `destroy` (if set and if built with it), then delete the entity |
| anything else     | Treated as a new `update` callback, installed for next frame         |

The callback only ever receives the payload, never the entity handle — recover it with `DARKEN_ENTITY(data)` if you need to touch `usr`/`tag`.

```c
void *player_walk(struct Player *p) { ... }
```

— assigns straight to `entity->update` with no cast. Only declare `void *data` for callbacks that genuinely ignore the payload.

`destroy` (when `DARKEN_NO_DESTROY` isn't defined) uses the same `darken_state_t` callback type and the same `(data)` argument convention as `update`. Its return value is always ignored — `darken_reset()` and `darken_entity_delete()` only ever call it for its side effects.

`destroy` must not mutate the ctx's pool zones. Deleting, spawning, migrating, or swapping entities from inside a `destroy` callback will corrupt the swap state and iteration that the engine relies on. This applies to every path that invokes `destroy`: `darken_reset()`, `darken_entity_delete()`, and the `DARKEN_DELETE` branch inside `darken_update()`.

Comparing a `darken_state_t` value against the sentinels relies on your target having a single flat address space (see [Requirements](#requirements)).

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

The `destroy` callback (when present) uses the same `darken_state_t` callback type and the same `(entity, data)` argument convention as `update`; its return value is ignored.

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

The old-style, unprototyped callback type is a common-and-widely-supported (but not ISO-C-guaranteed) calling-convention convenience. It is not a general guarantee that arbitrary incompatible function signatures may be interchanged safely.

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

`darken_entity_migrate()` is the tool for a "paused set": keep a second, smaller `darken_t` (it can even use a more compact payload struct than the active one — see Gotchas for what a stride mismatch does) that you simply never pass to `darken_update()`, and migrate entities into and out of it as needed. Its internal copy moves data in `DARKEN_MIGRATE_WORD_T`-sized chunks (`unsigned char` by default — see [Configuration](#configuration)) with a byte tail for whatever's left over, so it's correct for any word size you configure it to.

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
| `darken_entity_delete(entity)`                 | Destroys (if active, and if built with `destroy` support) and frees an entity      |
| `darken_entity_migrate(entity, dst)`           | Copies an entity into another `darken_t`; see above                                |
| `darken_entity_swap(e1, e2)`                   | Trades two entities' positions, even across two different ctx's                    |
| `darken_update(ctx)`                           | Runs one frame over the active zone                                                |
| `darken_reset(ctx)`                            | Destroys all active entities, then empties the pool                                |
| `DARKEN_FOREACH(ctx, code)`                    | Manual iteration over the active zone                                              |
| `DARKEN_ENTITY_IS_ACTIVE/FREE(entity)`         | Zone membership tests                                                              |
| `DARKEN_COUNT_ACTIVE/FREE(ctx)`                | Zone sizes                                                                         |

Configuration macros (`DARKEN_SMALL`, `DARKEN_INDEX_T`, `DARKEN_USR_T`, `DARKEN_TAG_T`, `DARKEN_NO_TAG`, `DARKEN_NO_USR`, `DARKEN_NO_DESTROY`, `DARKEN_MIGRATE_WORD_T`, `DARKEN_DIRECT`) are covered in [Configuration](#configuration), not repeated here.

## Gotchas

- `capacity`/`size`/`stride` are `darken_index_t` (`uint16_t` by default, `uint8_t` under `DARKEN_SMALL` or an explicit `DARKEN_INDEX_T`), so a single `darken_t` tops out at that type's max value, and the computed entity stride must also fit in it.

- **`update == NULL` is a hard crash, not a no-op.** There's no NULL check before calling `entity->update` — see [Two guarantees you can rely on](#two-guarantees-you-can-rely-on). Always assign it on every spawn.

- **`DARKEN_SPAWN`/`DARKEN_FOREACH` evaluate their `ctx` argument more than once.** `DARKEN_SPAWN(managers[idx++])` contains multiple unsequenced evaluations of `idx++` in one expression and is undefined behavior in C. `DARKEN_FOREACH(managers[idx++], ...)` evaluates its `ctx` argument twice in separate statements, so it is not the same undefined-behavior case, but `idx` still advances twice. Only ever pass a stable expression such as `&m` or a plain pointer variable to either macro.

- **`darken_entity_migrate()` is not a no-op on a free entity.** If `entity` is already free, its source ctx is left untouched, but `dst` still gains a new active entity built from `entity`'s (already-recycled, meaningless) payload bytes. Calling it by mistake on something you thought was already gone silently spends a slot in `dst`, it does not return early.

- **`darken_entity_migrate()` truncates or leaves stale data on a stride mismatch.** Exactly `min(src->stride, dst->stride)` bytes are copied. Moving into a smaller-stride ctx truncates the tail of the payload; moving into a larger-stride one leaves the payload's tail as whatever was already in that recycled slot. The header fields are copied first and then `slot`/`owner` are corrected for `dst`. If you rely on specific payload fields surviving a round trip through a smaller ctx, put those fields first in both payload structs.

- **No staleness detection.** Darken hands you raw pointers, not generation-checked handles. If you keep a `darken_entity_t` around after deleting it, and a later spawn reuses that exact slot, your old pointer now silently refers to the *new* occupant — same address, different entity. If you need to detect this, roll your own generation counter in `tag` or `usr` and check it yourself; Darken has no built-in way to ask "is this still the entity I think it is?".

- Fields aren't auto-initialized on spawn (see above) — an entity with a stale or invalid `update` pointer will crash the moment `darken_update()` reaches it.

- **`DARKEN_NO_TAG` / `DARKEN_NO_USR` / `DARKEN_NO_DESTROY` are semantic changes.** Any code touching a removed field simply fails to compile — that's the intended, compile-time feedback, not a bug to work around.

- **Payload alignment is a precondition.** The payload type used with `DARKEN_DATA()` must not require stricter alignment than `struct darken_entity_t`. See the ARM32-EABI note under [Configuration](#configuration) if you're using `uint64_t`-sized header fields.

- `darken_init()` resets the logical pool without calling `destroy()`. Do not call it on a live population unless that population is intentionally being discarded.

- `DARKEN_FREE()` releases storage only; it does not run `destroy()` callbacks. Call `darken_reset()` first when destructor side effects are required.

- `DARKEN_ALLOC()` does not handle allocation failure or partial allocation cleanup. Check its returned pointers before calling `darken_init()`.

- Toolchain and integer-type constraints (C89+FAM, `<stdint.h>`) are covered in [Requirements](#requirements), not repeated here.