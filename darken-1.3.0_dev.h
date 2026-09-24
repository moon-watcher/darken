/**
 * darken.h — Darken (DARKula ENgine) Entity System
 *
 * darken-1.3.0_dev
 *
 * Full documentation: README.Darken.md
 *
 * ============================================================================
 * PORTABILITY / REQUIREMENTS -- READ THIS FIRST
 * ============================================================================
 *
 * Darken is a generic, target-agnostic entity manager: nothing in its logic depends on any specific
 * platform, engine, or toolchain. It operates over plain caller-provided storage and pointers, so it drops
 * into any C project. Three things are required of whoever includes it, though:
 *
 * 1. Fixed-width integer types must already be visible BEFORE this header is included. Darken deliberately
 *    does NOT #include <stdint.h> itself -- the including project must provide them, whether via a plain
 *    `#include <stdint.h>` or whatever equivalent your target already defines them through.
 *
 * 2. A GNU C compiler -- GCC or Clang. Darken relies on the __attribute__((aligned)) extension
 *    (DARKEN_DECLARE), and __alignof__. It will not build under a strict ISO-C-only compiler. Sentinel
 *    handling in state-machine mode (== against DARKEN_CONTINUE and > against DARKEN_CONTINUE to detect new
 *    callbacks) additionally relies on GNU C / target-ABI behavior for converting small integer values to
 *    function pointers and comparing function-pointer values with those sentinels.
 *
 * 3. CAPACITY must satisfy 1 <= CAPACITY <= UINT16_MAX, and the computed entity stride must fit in uint16_t.
 *    These are API requirements; DARKEN_DECLARE() only rejects CAPACITY == 0 with a compile-time
 *    negative-size array. The caller is responsible for keeping CAPACITY and stride within uint16_t limits.
 *
 * Beyond that, Darken makes no assumptions about the target: pointer width, struct alignment, and endianness
 * are all whatever the compiler says they are for the platform it's building for (see __alignof__ in the
 * storage declarations and stride calculation rather than any hardcoded value).
 *
 * The payload type used with DARKEN_DATA() must not require stricter alignment than struct darken_entity_t
 * itself. Darken knows the payload size at storage declaration time, but not the payload type's alignment.
 *
 *
 * Entity: Base entity managed by the entity ctx
 *
 * The entity structure serves as a container for user data with lifecycle management. The flexible array
 * member 'data[]' allows entities to have variable-sized payloads while maintaining contiguous memory layout.
 *
 * The stride between entities is pre-calculated once, at ctx initialization, purely to lay out the fixed
 * storage block (darken_init() walks it in `stride`-sized steps to hand each entity its permanent address).
 * O(1) access by index comes from `pool[]` itself being a flat array of pointers -- stride plays no part in
 * any lookup after init.
 *
 * An entity's own memory address (this struct) never moves once allocated by darken_init(). What moves
 * between the ctx's zones is only the *pointer* to it inside darken.pool[]. This is what makes it safe to
 * keep a raw pointer into entity->data even while the entity gets reordered.
 *
 * The ctx itself needs the same guarantee, for the same reason: darken_init() bakes the address it was given
 * into every entity's ->owner, so the darken_t instance must already be sitting at its final address before
 * darken_init() runs, and must stay there for as long as the ctx is used. A helper that builds a darken_t on
 * its own stack and returns it *by value* will compile and look fine, but leaves every entity's ->owner
 * dangling into a stack frame that no longer exists. Build it in place, or pass a `darken_t *` in.
 *
 *
 * Ctx: Entity container and lifecycle ctx. Maintains the pointer array in two logical zones:
 *
 * Array Layout:
 *    [ active entities ][   free slots    ]
 *    0                 size               capacity
 *
 * The entities themselves live in the caller-provided storage block; ctx->pool contains pointers to those
 * fixed addresses.
 *
 * - Active zone [0, size):
 *     Entity pointers updated every frame by darken_update(). Iterable with DARKEN_FOREACH (note: iterates
 *     in reverse order, from size-1 down to 0). Freely created (DARKEN_SPAWN) and deleted.
 *
 * - Free zone [size, capacity):
 *     Pointer slots not currently assigned to an entity. This is where DARKEN_SPAWN() takes its next entity
 *     from, and where an active entity's slot goes right after it's deleted.
 *
 * Neither darken_init(), nor DARKEN_SPAWN(), nor deletion (whichever path triggers it) initializes or clears
 * update/destroy/tag/usr. An entity handed out by DARKEN_SPAWN() — whether fresh from darken_init() or
 * recycled after a previous entity in that slot was deleted — may still carry whatever values that slot's
 * previous occupant left behind. Setting these fields to the values your entity actually needs (including
 * clearing any you don't want carried over) is the caller's responsibility on every spawn.
 *
 *
 * Update / lifecycle control — two selectable modes
 * ==================================================
 *
 * STATE-MACHINE mode is the default. Define DARKEN_DIRECT before including this header to opt into direct
 * mode instead.
 *
 * Each mode has one fixed callback signature — there is no separate configuration macro for the argument
 * list.
 * The signature is chosen per mode to match how that mode is actually used: state-machine callbacks rarely
 * need the entity handle, since the return value drives the lifecycle; direct-mode callbacks almost always
 * need it, since they call darken_entity_delete() themselves.
 *
 * 1) STATE-MACHINE mode — default
 * ---------------------------------------------------------
 *     Signature: darken_state_t callback(void *data)
 *
 *     Only the entity's payload is passed — never the entity handle. darken_update() reads update()'s return
 *     value and drives the lifecycle itself:
 *
 *         DARKEN_CONTINUE: stay active, keep the same update callback
 *         DARKEN_DELETE:   call destroy (if set), then delete the entity
 *         (anything else): treated as a new update callback pointer; installed as entity->update for next frame
 *
 *         void *player_walk_state(struct player *data)
 *         {
 *             data->x++;
 *
 *             if (should_stop(data))
 *                 return player_stop_state;
 *
 *             if (should_die(data))
 *                 return DARKEN_DELETE;
 *
 *             return DARKEN_CONTINUE;
 *         }
 *
 *     `destroy` uses the same callback type and the same (data)-only argument convention as `update`.
 *     Its return value is always ignored — darken_reset() and darken_entity_delete() only ever call it
 *     for its side effects.
 *
 *     `destroy` must not mutate the ctx's pool zones. Deleting, spawning, or otherwise reordering entities
 *     from inside a destroy callback will corrupt the swap state and iteration that the engine relies on.
 *     This restriction applies to every path that invokes destroy: darken_reset(), darken_entity_delete(),
 *     and the DARKEN_DELETE branch inside darken_update().
 *
 *     Need the entity handle anyway (e.g. to read/write usr or tag)? Recover it with DARKEN_ENTITY(data).
 *
 *     Comparing a darken_state_t value against the sentinels with `==`/`>` relies on GNU C / target-ABI
 *     behavior for the function-pointer sentinel representation described in requirement 2 above.
 *
 * 2) DIRECT mode — DARKEN_DIRECT defined
 * ---------------------------------------------------------
 *     Signature: void callback(darken_entity_t entity)
 *                void callback(darken_entity_t entity, void *data)
 *
 *     Both the entity handle and its payload are passed, in that order. darken_update() just calls
 *     entity->update(entity, entity->data) every frame and ignores any return value; entity->destroy(entity,
 *     entity->data) is called the same way by darken_reset() and darken_entity_delete(). The callback is in
 *     full control of the entity's lifecycle: it changes state by assigning directly to entity->update
 *     (and/or entity->destroy), and it deletes itself by calling darken_entity_delete() — which takes the
 *     handle it was just given directly, no DARKEN_ENTITY(data) lookup needed.
 *
 *         void player_walk_state(darken_entity_t entity) {
 *             DARKEN_DATA(struct player, data, entity);
 *             data->x++;
 *         }
 *
 *         void player_walk_events_state(darken_entity_t entity, struct player *data) {
 *             data->x++;
 *
 *             if (should_stop(data))
 *                 entity->update = player_stop_state;
 *
 *             if (should_die(data))
 *                 darken_entity_delete(entity);
 *         }
 *
 *     A callback that only declares the entity parameter (e.g. `void f(darken_entity_t entity)`) still works:
 *     darken_state_t has no prototype, so the callee just reads however many leading arguments it declares and
 *     the rest are pushed and ignored. This is a GNU-C/ABI-oriented convenience, not a general ISO-C rule.
 */

#pragma once

// Darken does NOT include <stdint.h> itself -- see "PORTABILITY / REQUIREMENTS" at the top of this file.
// Make sure uint8_t/uint16_t/uint32_t/uintptr_t are visible before this point: either `#include <stdint.h>`
// yourself, or rely on whatever your platform already provides in its place.

#ifdef DARKEN_DIRECT
typedef void (*darken_state_t)();
#else
typedef void *(*darken_state_t)();
#endif

typedef struct darken_entity_t *darken_entity_t;

typedef struct darken_t
{
    darken_entity_t *pool; // Pointer array to entities in the ctx's storage block
    uint8_t *storage;      // Pointer to the contiguous memory block where entities are allocated
    uint16_t capacity;
    uint16_t size;
    uint16_t stride;
} darken_t;

struct darken_entity_t
{
    uint16_t slot;          // Private: Index in the ctx's pool array
    uint16_t usr;           // User-defined field for custom data
    darken_state_t update;  // User-defined update callback
    darken_state_t destroy; // User-defined destroy callback
    uint32_t tag;           // User-defined tag for identification or categorization
    darken_t *owner;        // Private: Pointer to the owning ctx
    uint8_t data[];         // Payload
};

/* ============================================================================
 * PRIVATE
 * ============================================================================ */

// Single call-site helper for invoking update()/destroy(), used everywhere the engine calls into user code.
// The argument list is fixed per mode (see the big comment above), so there is nothing to configure here.
#ifdef DARKEN_DIRECT
#define _DARKEN_ARGS(ENTITY) (ENTITY), (ENTITY)->data
#else
#define _DARKEN_ARGS(ENTITY) (ENTITY)->data
#endif

#define _DARKEN_ENTITY_STRIDE(PAYLOAD) (((sizeof(struct darken_entity_t) + (PAYLOAD)) + (uintptr_t)__alignof__(struct darken_entity_t) - 1) & ~((uintptr_t)__alignof__(struct darken_entity_t) - 1))

/* ============================================================================
 * PUBLIC API
 * ============================================================================ */

// Sentinel return values for update() callbacks in state-machine mode.
// Any value > DARKEN_CONTINUE is treated as the next update callback.
#define DARKEN_CONTINUE ((darken_state_t)1)
#define DARKEN_DELETE ((darken_state_t)0)

// Dynamic allocation: use with malloc/calloc or custom allocator
//     darken_t m = DARKEN_ALLOC(malloc, 5, sizeof(struct MyComponent));
//     if (!m.pool || !m.storage) return;
//     darken_init(&m);
//     ...
//     DARKEN_FREE(free, &m);
//
// DARKEN_ALLOC() does not handle allocation failure or partial allocation cleanup.
// CAPACITY must satisfy 1 <= CAPACITY <= UINT16_MAX, and the computed stride must fit in uint16_t.
// DARKEN_ALLOC() does not perform explicit validation of these requirements, so the caller is
// responsible for providing valid values.
#define DARKEN_ALLOC(ALLOC, CAPACITY, PAYLOAD)                           \
    (darken_t)                                                           \
    {                                                                    \
        .pool = (ALLOC)((CAPACITY) * sizeof(darken_entity_t)),           \
        .storage = (ALLOC)((CAPACITY) * _DARKEN_ENTITY_STRIDE(PAYLOAD)), \
        .capacity = (CAPACITY),                                          \
        .stride = _DARKEN_ENTITY_STRIDE(PAYLOAD),                        \
    }

// Frees the pool and storage blocks previously allocated by DARKEN_ALLOC().
// DARKEN_FREE() only releases memory; it does not call destroy() or reset the ctx.
#define DARKEN_FREE(FREE, CTX)  \
    do                          \
    {                           \
        (FREE)((CTX)->pool);    \
        (FREE)((CTX)->storage); \
    } while (0)

// Static storage declaration (stack or global)
//     DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
//     darken_t m = DARKEN_BIND(storage);
//     darken_init(&m);
//
// CAPACITY must satisfy 1 <= CAPACITY <= UINT16_MAX. The payload type used with DARKEN_DATA() must not
// require stricter alignment than struct darken_entity_t. DARKEN_DECLARE() rejects CAPACITY == 0 with a
// compile-time negative-size array. The caller is responsible for keeping CAPACITY and the computed
// entity stride within their uint16_t limits.
#define DARKEN_DECLARE(NAME, CAPACITY, PAYLOAD)                                                                                  \
    struct                                                                                                                       \
    {                                                                                                                            \
        uint16_t capacity;                                                                                                       \
        uint16_t stride;                                                                                                         \
        darken_entity_t pool[(CAPACITY) ? (CAPACITY) : -1] __attribute__((aligned(__alignof__(darken_entity_t))));               \
        uint8_t data[(CAPACITY) * _DARKEN_ENTITY_STRIDE(PAYLOAD)] __attribute__((aligned(__alignof__(struct darken_entity_t)))); \
    } NAME = {                                                                                                                   \
        .capacity = (CAPACITY),                                                                                                  \
        .stride = _DARKEN_ENTITY_STRIDE(PAYLOAD),                                                                                \
    }

// Static/global initialization: compile-time constants.
// Use when the storage is defined at file scope and you want static initialization.
// NOTE: DARKEN_INIT expands to a compound literal, which is not a constant expression in strict C99 -- it
// requires C11 (or the GNU C extension) to initialize an object with static storage duration. This matches
// the GNU C requirement already documented at the top of this file.
#define DARKEN_INIT(STORAGE)                                                                   \
    (darken_t)                                                                                 \
    {                                                                                          \
        .pool = (STORAGE).pool,                                                                \
        .storage = (STORAGE).data,                                                             \
        .capacity = sizeof((STORAGE).pool) / sizeof(darken_entity_t),                          \
        .stride = sizeof((STORAGE).data) / (sizeof((STORAGE).pool) / sizeof(darken_entity_t)), \
    }

// Runtime binding: locals, reassignment, any context
// Use when you need to (re)bind a darken_t context to storage at runtime
#define DARKEN_BIND(NAME)            \
    (darken_t)                       \
    {                                \
        .pool = (NAME).pool,         \
        .storage = (NAME).data,      \
        .capacity = (NAME).capacity, \
        .stride = (NAME).stride,     \
    }

// Spawn a new entity from the free zone. Returns the entity or 0 if no free slots.
// The returned entity may contain garbage from a previous occupant — always initialize all fields you care
// about (update, destroy, tag, usr, and data).
#define DARKEN_SPAWN(CTX) ((CTX)->size < (CTX)->capacity ? (CTX)->pool[(CTX)->size++] : 0)

// Iterate over all active entities in REVERSE order (from size-1 down to 0).
// Reverse iteration makes deleting the currently visited entity safe.
// Deleting or reordering other entities from inside CODE can affect which entities are visited in this pass.
#define DARKEN_FOREACH(CTX, CODE)                    \
    do                                               \
    {                                                \
        uint16_t _index = (CTX)->size;               \
        darken_entity_t *_pool = (CTX)->pool;        \
                                                     \
        while (_index--)                             \
        {                                            \
            darken_entity_t _entity = _pool[_index]; \
            CODE;                                    \
        }                                            \
    } while (0)

// Declare a typed pointer to an entity's data payload
#define DARKEN_DATA(TYPE, VAR, ENTITY) TYPE *VAR = (TYPE *)(ENTITY)->data;

// Recover the entity handle from a pointer to its data payload (Mostly useful in STATE-MACHINE mode where
// callbacks only receive data).
// DATA must point to the beginning of an entity's data[] payload.
// Note: uses the standard "offsetof-via-null-pointer" idiom, which is formally undefined behaviour but
// works on GCC/Clang (already required by this header).
#define DARKEN_ENTITY(DATA) ((darken_entity_t)((uint8_t *)(DATA) - (uintptr_t)&((darken_entity_t)0)->data))

// Zone membership tests.
// Note: ENTITY is evaluated multiple times per test (up to two times for DARKEN_ENTITY_IS_FREE). Do not
// pass expressions with side effects.
#define DARKEN_ENTITY_IS_ACTIVE(ENTITY) ((ENTITY)->slot < (ENTITY)->owner->size)
#define DARKEN_ENTITY_IS_FREE(ENTITY) (!DARKEN_ENTITY_IS_ACTIVE(ENTITY))

// Zone sizes, so callers don't have to do the size/capacity math by hand.
#define DARKEN_COUNT_ACTIVE(CTX) ((CTX)->size)
#define DARKEN_COUNT_FREE(CTX) ((uint16_t)((CTX)->capacity - (CTX)->size))

/* ============================================================================
 * FUNCTIONS
 * ============================================================================ */

// Swap two entities in their owners' pool arrays. The entities may belong to the same ctx or to different
// ctx's. Each entity is exchanged with the pool slot it currently occupies in its own owner, then both owner
// pointers and slot indices are exchanged with the pool entries. This keeps ->owner and ->slot consistent
// even when the two entities cross ctx boundaries.
//
// Takes the entities themselves rather than (ctx, i, j) so callers cannot pass a mismatched ctx or stale
// index: each ctx and index are recovered directly from the entity being moved.
static inline void darken_entity_swap(darken_entity_t e1, darken_entity_t e2)
{
    if (e1 == e2)
        return;

    darken_t *ctx1 = e1->owner;
    darken_t *ctx2 = e2->owner;
    uint16_t i = e1->slot;
    uint16_t j = e2->slot;

    ctx1->pool[i] = e2;
    ctx2->pool[j] = e1;
    e1->slot = j;
    e2->slot = i;
    e1->owner = ctx2;
    e2->owner = ctx1;
}

// Note: darken_entity_delete() only calls destroy() if the entity is active.
// destroy() must not mutate the ctx's pool zones (delete/spawn) while it runs -- see the big header comment
// above.
static inline void darken_entity_delete(darken_entity_t entity)
{
    if (DARKEN_ENTITY_IS_FREE(entity))
        return;

    if (entity->destroy)
        entity->destroy(_DARKEN_ARGS(entity));

    darken_entity_swap(entity, entity->owner->pool[--entity->owner->size]);
}

// Migrate `entity` from its current ctx into `dst`, WITHOUT calling destroy().
// Returns the entity as it now lives in `dst`, or 0 if the transfer cannot be performed.
//
// `src` and `dst` may use different strides. Exactly min(src->stride, dst->stride) bytes are copied.
// If dst is smaller, excess payload data is truncated; if dst is larger, the remaining payload is unchanged.
// The common header fields are preserved, except slot and owner, which are updated for `dst`.
//
// If `entity` is active, it is removed from `src` after the copy. If it is free, its source slot remains free.
// Note this is NOT a no-op when entity is free: dst still gains a new active entity, built from whatever
// (already-recycled) bytes were sitting in entity's payload. Calling this on a free entity by mistake will
// silently spend a slot in dst with garbage data, not return early.
//
// The copy runs in 32-bit words with a byte tail for the remainder. The uint32_t accesses are safe without
// any runtime alignment check: entities are laid out at storage + i * stride, where storage is aligned to
// __alignof__(struct darken_entity_t) and stride is a multiple of it. On 68000 a move.l only requires an
// even address, and __alignof__(struct darken_entity_t) is at least 2 on every target Darken builds for.
// On any target with stricter uint32_t alignment, __alignof__(struct darken_entity_t) would already be >= 4
// because struct darken_entity_t contains a uint32_t member, so the access remains aligned there too.
static inline darken_entity_t darken_entity_migrate(darken_entity_t entity, darken_t *dst)
{
    darken_t *src = entity->owner;

    if (src == dst || dst->size >= dst->capacity)
        return 0;

    uint16_t count = src->stride < dst->stride ? src->stride : dst->stride;
    uint16_t words = count >> 2;
    uint16_t dst_slot = dst->size++;
    darken_entity_t moved = dst->pool[dst_slot];
    uint32_t *d = (uint32_t *)moved;
    uint32_t *s = (uint32_t *)entity;

    while (words--)
        *d++ = *s++;

    uint8_t *sb = (uint8_t *)s;
    uint8_t *db = (uint8_t *)d;

    count &= 3;
    while (count--)
        *db++ = *sb++;

    moved->slot = dst_slot;
    moved->owner = dst;

    if (DARKEN_ENTITY_IS_ACTIVE(entity))
        darken_entity_swap(entity, src->pool[--src->size]);

    return moved;
}

//
// USAGE EXAMPLES:
//
// DYNAMIC:
//     darken_t m = DARKEN_ALLOC(malloc, 5, sizeof(struct MyComponent));
//     if (!m.pool || !m.storage) return;
//     darken_init(&m);
//     ...
//     darken_reset(&m);
//     DARKEN_FREE(free, &m);
//
// STATIC (Runtime binding):
// Runtime: locals, reassignment, any context
//     DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
//     darken_t m = DARKEN_BIND(storage);
//     darken_init(&m);
//
// STATIC (Compile-time initialization):
// Static/global initialization: compile-time constants
//     DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
//     static darken_t m = DARKEN_INIT(storage);
//
//     void init_test_manager() {
//         darken_init(&m);
//         ...
//     }
//
// darken_init() must only be called on unused/uninitialized storage, or after darken_reset() when the
// current population is intentionally being discarded. It does not call destroy() for the entities already
// managed by the ctx.
//
// Walks the capacity-sized storage block once, handing each pool slot a permanent address. The loop counts
// down (capacity-1 to 0) rather than up; the direction itself is not significant and does not imply slot i
// lives at storage offset i*stride. Only ->slot and ->owner, not array position, are guaranteed to track an
// entity afterward.
static inline void darken_init(darken_t *ctx)
{
    ctx->size = 0;
    uint16_t i = ctx->capacity;
    uint8_t *storage = ctx->storage;

    while (i--)
    {
        ctx->pool[i] = (darken_entity_t)storage;
        ctx->pool[i]->owner = ctx;
        ctx->pool[i]->slot = i;

        storage += ctx->stride;
    }
}

static inline void darken_update(darken_t *ctx)
{
#ifdef DARKEN_DIRECT
    DARKEN_FOREACH(ctx, _entity->update(_DARKEN_ARGS(_entity)));
#else
    DARKEN_FOREACH(ctx, {
        darken_state_t state = _entity->update(_DARKEN_ARGS(_entity));

        if (state == DARKEN_CONTINUE)
            continue;

        if (state > DARKEN_CONTINUE)
        {
            _entity->update = state;
            continue;
        }

        if (_entity->destroy)
            _entity->destroy(_DARKEN_ARGS(_entity));

        darken_entity_swap(_entity, ctx->pool[--ctx->size]);
    });
#endif
}

// Calls destroy() on every currently active entity, then drops the whole pool back to the free zone
// (size = 0).
//
// destroy() callbacks used by darken_reset() must not mutate the ctx's pool zones by deleting, spawning,
// or otherwise reordering entities during the reset iteration. (This is a specific case of the general
// restriction documented in the big header comment above.)
static inline void darken_reset(darken_t *ctx)
{
    DARKEN_FOREACH(ctx, {
        if (_entity->destroy)
            _entity->destroy(_DARKEN_ARGS(_entity));
    });

    ctx->size = 0;
}
