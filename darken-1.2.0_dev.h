/**
 * darken.h — Darken (DARKula ENgine) Entity System
 *
 * darken-1.2.0_dev
 *
 * ============================================================================
 * PORTABILITY / REQUIREMENTS -- READ THIS FIRST
 * ============================================================================
 *
 * Darken is a generic, target-agnostic entity manager: nothing in its logic depends
 * on any specific platform, engine, or toolchain. It operates over plain caller-
 * provided storage and pointers, so it drops into any C project. Three things are
 * required of whoever includes it, though:
 *
 * 1. Fixed-width integer types must already be visible BEFORE this header is
 *    included. Darken deliberately does NOT #include <stdint.h> itself -- the
 *    including project must provide them, whether via a plain `#include <stdint.h>`
 *    or whatever equivalent your target already defines them through.
 *
 * 2. A GNU C compiler -- GCC or Clang. Darken relies on GNU C statement expressions
 *    (DARKEN_SPAWN*), the __attribute__((aligned)) extension (DARKEN_DECLARE*), and
 *    __alignof__. It will not build under a strict ISO-C-only compiler.
 *
 * 3. CAPACITY must satisfy 1 <= CAPACITY <= UINT16_MAX, ZONES must satisfy
 *    1 <= ZONES <= DARKEN_MAX_ZONES, and the computed entity stride must fit in
 *    uint16_t. These are API requirements; DARKEN_DECLARE()/DARKEN_DECLARE_ZONES()
 *    enforce them at compile time via a negative-size array (see _darken_checks
 *    below). DARKEN_ALLOC()/DARKEN_ALLOC_ZONES() do NOT enforce them -- they expand
 *    to an expression, not a declaration -- so dynamic-allocation callers must
 *    verify them manually.
 *
 * ============================================================================
 * VERSIONING NOTE (1.1 -> 1.2)
 * ============================================================================
 *
 * The public API of 1.1 is preserved:
 *   - `destroy` is back in darken_entity_t and is invoked from the same points as
 *     in 1.1 (DARKEN_DELETE branch, darken_entity_delete, darken_reset).
 *   - DARKEN_ALLOC / DARKEN_DECLARE / DARKEN_INIT keep their original signatures
 *     (single-zone: ZONES defaults to 1, matching 1.1's single active zone).
 *   - DARKEN_FOREACH iterates over the entities in zone 0 (the default zone),
 *     reverse order. With a single-zone ctx this is the whole active set,
 *     matching 1.1. Multi-zone callers iterate other zones with
 *     DARKEN_FOREACH_ZONE.
 *   - darken_update(ctx) updates all user zones; darken_entity_delete destroys
 *     before recycling; darken_reset destroys everything before resetting.
 *
 * What is NOT coming back:
 *   - DARKEN_PAUSE. Update callbacks no longer return a third sentinel. There is
 *     no universal notion of "paused" in the engine. If the application wants a
 *     paused set, it defines a user zone and simply does not update that zone.
 *     Moving an entity into such a zone is done with darken_entity_set_zone().
 *   - DARKEN_ENTITY_IN_PAUSED / DARKEN_COUNT_PAUSED. There is no universal notion
 *     of "paused" in the engine; use DARKEN_ENTITY_IN_ZONE(entity, Z) and
 *     DARKEN_COUNT_ZONE(ctx, Z).
 *   - darken_entity_pause / darken_entity_resume. Zone meaning belongs entirely
 *     to the application, so there is no universal pause/resume operation in the
 *     engine.
 *
 * Zone-aware variants are exposed as `*_zone` / `*_ZONE` / `*_ZONES`:
 *   DARKEN_ALLOC_ZONES, DARKEN_DECLARE_ZONES, DARKEN_SPAWN_ZONE,
 *   DARKEN_FOREACH_ZONE, darken_update_zone, darken_reset_zone,
 *   darken_entity_zone, darken_entity_set_zone, darken_count_zone,
 *   DARKEN_ENTITY_IN_ZONE, DARKEN_COUNT_ZONE.
 *
 * PERFORMANCE NOTE: DARKEN_SPAWN and darken_entity_delete special-case
 * `ctx->zones == 1` to bypass the generic zone-movement machinery, restoring
 * the O(1) single-swap behavior of 1.1 for the common single-zone case.
 * Multi-zone ctxs still pay the zone-movement cost.
 *
 * ============================================================================
 * Entity: Base entity managed by the ctx
 * ============================================================================
 *
 * The entity structure serves as a container for user data with lifecycle
 * management. The flexible array member 'data[]' allows entities to have
 * variable-sized payloads while maintaining contiguous memory layout.
 *
 * An entity's own memory address never moves once allocated by darken_init().
 * What moves between the ctx's zones is only the *pointer* to it inside
 * darken.pool[]. This is what makes it safe to keep a raw pointer into
 * entity->data even while the entity is moved between zones.
 *
 * The ctx itself needs the same guarantee, for the same reason: darken_init()
 * bakes the address it was given into every entity's ->owner, so the darken_t
 * instance must already be sitting at its final address before darken_init()
 * runs, and must stay there for as long as the ctx is used. Build it in place,
 * or pass a `darken_t *` in.
 *
 * ============================================================================
 * Ctx: Entity container and lifecycle ctx
 * ============================================================================
 *
 * The pointer array is partitioned into `zones` user zones followed by a free
 * zone. bounds[z-1] is the lower edge of zone z; bounds[zones-1] is the upper
 * edge of the last user zone, which is exactly the old "size" from 1.1.
 *
 *    [ user zone 0 ][ user zone 1 ]...[ user zone N-1 ][   free slots   ]
 *    0            b[0]             b[1]              b[N-1]         capacity
 *
 * "size" (the number of active entities across all user zones) is derived from
 * bounds[zones-1] via _darken_size() and is never stored separately, so it
 * cannot desync from the actual layout.
 *
 * ============================================================================
 * Update / lifecycle control — two selectable modes
 * ============================================================================
 *
 * STATE-MACHINE mode is the default. Define DARKEN_DIRECT before including this
 * header to opt into direct mode instead.
 *
 * 1) STATE-MACHINE mode — default
 *     Signature: darken_state_t callback(void *data)
 *
 *         DARKEN_CONTINUE: stay active, keep the same update callback
 *         DARKEN_DELETE:   call destroy (if set), then delete the entity
 *         (anything else): treated as a new update callback pointer
 *
 *     `destroy` uses the same callback type and the same (data)-only argument
 *     convention as `update`. Its return value is always ignored.
 *
 *     `destroy` must not mutate the ctx's zones. Deleting, moving between zones,
 *     spawning, or otherwise reordering entities from inside a destroy callback
 *     will corrupt the swap state and iteration that the engine relies on. This
 *     restriction applies to every path that invokes destroy.
 *
 *     Need the entity handle anyway? Recover it with DARKEN_ENTITY(data).
 *
 * 2) DIRECT mode — DARKEN_DIRECT defined
 *     Signature: void callback(darken_entity_t entity)
 *                void callback(darken_entity_t entity, void *data)
 *
 *     Both the entity handle and its payload are passed, in that order.
 *     darken_update() calls entity->update(entity, entity->data) every frame
 *     and ignores any return value; entity->destroy(entity, entity->data) is
 *     called the same way.
 */

#pragma once

// Darken does NOT include <stdint.h> itself -- see "PORTABILITY / REQUIREMENTS".
// Make sure uint8_t/uint16_t/uint32_t/uintptr_t are visible before this point.

#ifndef DARKEN_MAX_ZONES
#define DARKEN_MAX_ZONES 8
#endif

#ifdef DARKEN_DIRECT
typedef void (*darken_state_t)();
#else
typedef void *(*darken_state_t)();
#endif

typedef struct darken_entity_t *darken_entity_t;

typedef struct darken_t
{
    darken_entity_t *pool;
    uint8_t *storage;
    uint16_t capacity;
    uint16_t stride;
    uint16_t zones;
    uint16_t bounds[DARKEN_MAX_ZONES]; // bounds[zones-1] == "size"
} darken_t;

struct darken_entity_t
{
    uint16_t slot;
    uint16_t usr;
    darken_state_t update;
    darken_state_t destroy;
    uint32_t tag;
    darken_t *owner;
    uint8_t data[];
};

/* ============================================================================
 * PRIVATE
 * ============================================================================ */

#define _DARKEN_ALIGN(X, A) (((X) + (uintptr_t)(A) - 1) & ~((uintptr_t)(A) - 1))
#define _DARKEN_POOL_ALIGN __alignof__(darken_entity_t)
#define _DARKEN_ENTITY_ALIGN __alignof__(struct darken_entity_t)
#define _DARKEN_ENTITY_STRIDE(PAYLOAD) _DARKEN_ALIGN(sizeof(struct darken_entity_t) + (PAYLOAD), _DARKEN_ENTITY_ALIGN)

#ifdef DARKEN_DIRECT
#define _DARKEN_ARGS(ENTITY) (ENTITY), (ENTITY)->data
#define _DARKEN_UPDATE _entity->update(_DARKEN_ARGS(_entity))
#else
#define _DARKEN_ARGS(ENTITY) (ENTITY)->data

#define _DARKEN_UPDATE                                             \
    darken_state_t state = _entity->update(_DARKEN_ARGS(_entity)); \
                                                                   \
    if (state == DARKEN_CONTINUE)                                  \
        continue;                                                  \
                                                                   \
    if (state > DARKEN_CONTINUE)                                   \
        _entity->update = state;                                   \
                                                                   \
    else /* DARKEN_DELETE */                                       \
    {                                                              \
        if (_entity->destroy)                                      \
            _entity->destroy(_DARKEN_ARGS(_entity));               \
                                                                   \
        _darken_move_free(_entity);                                \
    }
#endif

static inline void darken_swap(darken_entity_t pool[], uint16_t i, uint16_t j)
{
    if (i == j)
        return;

    darken_entity_t tmp = pool[i];
    pool[i] = pool[j];
    pool[j] = tmp;
    pool[i]->slot = i;
    pool[j]->slot = j;
}

// Unica fuente de verdad para "size": derivado de bounds[zones-1].
static inline uint16_t _darken_size(darken_t *ctx)
{
    return ctx->bounds[ctx->zones - 1];
}

// NOTE: moved here (right after _darken_size, before any of the functions
// below that call them) because these two are used inside darken_entity_zone(),
// _darken_move_zone(), _darken_move_from_free() and _darken_move_free(), all
// defined further down in this same PRIVATE section. A macro must be #defined
// before its point of use in the file.
#define DARKEN_ENTITY_IN_ACTIVE(ENTITY) ((ENTITY)->slot < _darken_size((ENTITY)->owner))
#define DARKEN_ENTITY_IN_FREE(ENTITY) ((ENTITY)->slot >= _darken_size((ENTITY)->owner))
// Note: ENTITY is evaluated multiple times in the macros above (and in
// DARKEN_ENTITY_IN_ZONE below, up to four times). Do not pass expressions with
// side effects.

static inline uint16_t _darken_zone_lo(darken_t *ctx, uint16_t zone)
{
    return zone > 0 ? ctx->bounds[zone - 1] : 0;
}

static inline uint16_t _darken_zone_hi(darken_t *ctx, uint16_t zone)
{
    return zone < ctx->zones ? ctx->bounds[zone] : ctx->capacity;
}

static inline uint16_t darken_entity_zone(darken_entity_t entity)
{
    darken_t *ctx = entity->owner;
    uint16_t z = 0;

    if (DARKEN_ENTITY_IN_FREE(entity))
        return ctx->zones;

    while (z < ctx->zones && entity->slot >= ctx->bounds[z])
        z++;

    return z;
}

static inline void _darken_move_zone(darken_entity_t entity, uint16_t target)
{
    if (DARKEN_ENTITY_IN_FREE(entity))
        return;

    uint16_t cur = darken_entity_zone(entity);

    if (cur == target)
        return;

    darken_t *ctx = entity->owner;

    while (cur < target)
        darken_swap(ctx->pool, entity->slot, --ctx->bounds[cur++]);

    while (cur > target)
        darken_swap(ctx->pool, entity->slot, ctx->bounds[--cur]++);
}

static inline void _darken_move_from_free(darken_entity_t entity, uint16_t target)
{
    if (!DARKEN_ENTITY_IN_FREE(entity))
        return;

    darken_t *ctx = entity->owner;
    uint16_t cur = ctx->zones;

    while (cur > target)
        darken_swap(ctx->pool, entity->slot, ctx->bounds[--cur]++);
}

static inline void _darken_move_free(darken_entity_t entity)
{
    if (DARKEN_ENTITY_IN_FREE(entity))
        return;

    uint16_t cur = darken_entity_zone(entity);
    darken_t *ctx = entity->owner;

    while (cur < ctx->zones)
        darken_swap(ctx->pool, entity->slot, --ctx->bounds[cur++]);
}

/* ============================================================================
 * PUBLIC API
 * ============================================================================ */

// Sentinel return values for update() callbacks in state-machine mode.
// Any other darken_state value returned is treated as the next update callback.
// NOTE: DARKEN_PAUSE is intentionally not provided -- see the versioning note
// at the top of the file.
#define DARKEN_CONTINUE ((darken_state_t)1)
#define DARKEN_DELETE ((darken_state_t)0)

/* --- ALLOC / DECLARE / INIT / BIND ----------------------------------------- */

// 1.1-compatible (one user zone: the default).
#define DARKEN_ALLOC(ALLOC, CAPACITY, PAYLOAD) DARKEN_ALLOC_ZONES((ALLOC), (CAPACITY), 1, (PAYLOAD))

#define DARKEN_ALLOC_ZONES(ALLOC, CAPACITY, ZONES, PAYLOAD)              \
    (darken_t)                                                           \
    {                                                                    \
        .pool = (ALLOC)((CAPACITY) * sizeof(darken_entity_t)),           \
        .storage = (ALLOC)((CAPACITY) * _DARKEN_ENTITY_STRIDE(PAYLOAD)), \
        .capacity = (CAPACITY),                                          \
        .stride = _DARKEN_ENTITY_STRIDE(PAYLOAD),                        \
        .zones = (ZONES),                                                \
    }

#define DARKEN_FREE(FREE, CTX)  \
    do                          \
    {                           \
        (FREE)((CTX)->pool);    \
        (FREE)((CTX)->storage); \
    } while (0)

// 1.1-compatible: storage with one user zone.
#define DARKEN_DECLARE(NAME, CAPACITY, PAYLOAD) DARKEN_DECLARE_ZONES(NAME, CAPACITY, 1, PAYLOAD)

// Static-storage declaration. Expands to a SINGLE declaration so it can be
// prefixed with `static` at file scope:
//
//     static DARKEN_DECLARE(storage, 5, sizeof(struct Foo));
//     static DARKEN_DECLARE_ZONES(storage, 5, 4, sizeof(struct Foo));
//
// Compile-time checks (_darken_checks[]) use a negative-size array rather than
// _Static_assert: _Static_assert cannot be prefixed with `static`, and putting
// it outside the struct would break the single-declaration requirement.
//
// _darken_zones_marker[] exists ONLY so DARKEN_INIT() can recover ZONES via
// sizeof() -- a genuine compile-time constant, since sizeof of an array member
// depends on its TYPE, not its runtime value. The scalar `zones` field is kept
// for DARKEN_BIND(), which runs at runtime and has no such restriction.
#define DARKEN_DECLARE_ZONES(NAME, CAPACITY, ZONES, PAYLOAD)                                                                                                               \
    struct                                                                                                                                                                 \
    {                                                                                                                                                                      \
        uint16_t capacity;                                                                                                                                                 \
        uint16_t stride;                                                                                                                                                   \
        uint16_t zones;                                                                                                                                                    \
        darken_entity_t pool[(CAPACITY) ? (CAPACITY) : -1] __attribute__((aligned(_DARKEN_POOL_ALIGN)));                                                                   \
        uint8_t data[(CAPACITY) * _DARKEN_ENTITY_STRIDE(PAYLOAD)] __attribute__((aligned(_DARKEN_ENTITY_ALIGN)));                                                          \
        uint8_t _darken_zones_marker[(ZONES) > 0 ? (ZONES) : 1];                                                                                                           \
        uint8_t _darken_checks[((ZONES) > 0 && (ZONES) <= DARKEN_MAX_ZONES) && ((CAPACITY) <= (uint16_t)-1) && (_DARKEN_ENTITY_STRIDE(PAYLOAD) <= (uint16_t)-1) ? 1 : -1]; \
    } NAME = {                                                                                                                                                             \
        .capacity = (CAPACITY),                                                                                                                                            \
        .stride = _DARKEN_ENTITY_STRIDE(PAYLOAD),                                                                                                                          \
        .zones = (ZONES),                                                                                                                                                  \
    }

// 1.1-compatible: zones is recovered from the storage block via sizeof().
#define DARKEN_INIT(STORAGE)                                                                   \
    (darken_t)                                                                                 \
    {                                                                                          \
        .pool = (STORAGE).pool,                                                                \
        .storage = (STORAGE).data,                                                             \
        .capacity = sizeof((STORAGE).pool) / sizeof(darken_entity_t),                          \
        .stride = sizeof((STORAGE).data) / (sizeof((STORAGE).pool) / sizeof(darken_entity_t)), \
        .zones = sizeof((STORAGE)._darken_zones_marker),                                       \
    }

// Runtime binding: locals, reassignment, any context.
#define DARKEN_BIND(NAME)            \
    (darken_t)                       \
    {                                \
        .pool = (NAME).pool,         \
        .storage = (NAME).data,      \
        .capacity = (NAME).capacity, \
        .stride = (NAME).stride,     \
        .zones = (NAME).zones,       \
    }

/* --- SPAWN ----------------------------------------------------------------- */

// 1.1-compatible: spawns into the default zone.
#define DARKEN_SPAWN(CTX) DARKEN_SPAWN_ZONE((CTX), 0)

// Fast path: with a single user zone, moving the entity from the free zone
// into zone 0 is just `bounds[0]++` (the entity already sits at pool[size],
// which is pool[bounds[0]]). Bypassing _darken_move_from_free() recovers
// essentially all of 1.1's spawn cost for single-zone contexts.
#define DARKEN_SPAWN_ZONE(CTX, ZONE) ({                                 \
    darken_t *_ctx = (CTX);                                             \
    uint16_t _zone = (ZONE);                                            \
    uint16_t _s = _darken_size(_ctx);                                   \
    darken_entity_t _entity = _s < _ctx->capacity ? _ctx->pool[_s] : 0; \
                                                                        \
    if (_entity)                                                        \
    {                                                                   \
        if (_ctx->zones == 1)                                           \
            _ctx->bounds[0]++;                                          \
        else                                                            \
            _darken_move_from_free(_entity, _zone);                     \
    }                                                                   \
                                                                        \
    _entity;                                                            \
})

/* --- FOREACH --------------------------------------------------------------- */

// Iterates over the entities in zone 0 (the default zone), reverse order.
// With a single-zone ctx (zones == 1) this is the whole active set, matching
// the 1.1 behavior. With multi-zone ctxs it visits only zone 0; use
// DARKEN_FOREACH_ZONE for any other zone.
#define DARKEN_FOREACH(CTX, CODE) DARKEN_FOREACH_ZONE(CTX, 0, CODE)

// Zone-restricted iteration. Reverse order within [lo, hi).
#define DARKEN_FOREACH_ZONE(CTX, ZONE, CODE)             \
    do                                                   \
    {                                                    \
        darken_t *_ctx = (CTX);                          \
        uint16_t _lo = _darken_zone_lo(_ctx, (ZONE));    \
        uint16_t _index = _darken_zone_hi(_ctx, (ZONE)); \
                                                         \
        if (_index > _lo)                                \
        {                                                \
            darken_entity_t *_pool = _ctx->pool;         \
            while (_index-- > _lo)                       \
            {                                            \
                darken_entity_t _entity = _pool[_index]; \
                CODE;                                    \
            }                                            \
        }                                                \
    } while (0)

/* --- DATA / ENTITY --------------------------------------------------------- */

// Declare a typed pointer to an entity's data payload.
#define DARKEN_DATA(TYPE, VAR, ENTITY) TYPE *VAR = (TYPE *)(ENTITY)->data;

// Recover the entity handle from a pointer to its data payload (mostly useful
// in STATE-MACHINE mode where callbacks only receive data).
// DATA must point to the beginning of an entity's data[] payload.
// Uses the offsetof-via-null-pointer idiom (formally UB, works on GCC/Clang).
#define DARKEN_ENTITY(DATA) ((darken_entity_t)((uint8_t *)(DATA) - (uintptr_t)&((darken_entity_t)0)->data))

/* --- ZONE QUERIES ---------------------------------------------------------- */

#define DARKEN_ENTITY_IN_ZONE(ENTITY, ZONE) (DARKEN_ENTITY_IN_ACTIVE(ENTITY) && (ENTITY)->slot >= _darken_zone_lo((ENTITY)->owner, (ZONE)) && (ENTITY)->slot < _darken_zone_hi((ENTITY)->owner, (ZONE)))
#define DARKEN_COUNT_ACTIVE(CTX) (_darken_size(CTX))
#define DARKEN_COUNT_FREE(CTX) ((uint16_t)((CTX)->capacity - _darken_size(CTX)))
#define DARKEN_COUNT_ZONE(CTX, ZONE) darken_count_zone((CTX), (ZONE))

static inline uint16_t darken_count_zone(darken_t *ctx, uint16_t zone)
{
    return _darken_zone_hi(ctx, zone) - _darken_zone_lo(ctx, zone);
}

/* --- LIFECYCLE ------------------------------------------------------------- */

// 1.1-compatible: destroys the entity (if destroy is set) and moves it back to
// the free zone. No-op if the entity is already free.
//
// Fast path: with a single user zone, moving the entity out of zone 0 into the
// free zone is a single swap against bounds[0]-1, exactly as 1.1 did. Bypassing
// _darken_move_free() (and therefore darken_entity_zone's bounds[] scan) recovers
// essentially all of 1.1's delete cost for single-zone contexts.
static inline void darken_entity_delete(darken_entity_t entity)
{
    if (DARKEN_ENTITY_IN_FREE(entity))
        return;

    if (entity->destroy)
        entity->destroy(_DARKEN_ARGS(entity));

    darken_t *ctx = entity->owner;

    if (ctx->zones == 1)
        darken_swap(ctx->pool, entity->slot, --ctx->bounds[0]);
    else
        _darken_move_free(entity);
}

// Zone-aware move. No-op if the entity is already in `zone`, or if it is in the
// free zone.
static inline void darken_entity_set_zone(darken_entity_t entity, uint16_t zone)
{
    _darken_move_zone(entity, zone);
}

/* --- CTX LIFECYCLE --------------------------------------------------------- */

// Must only be called on unused/uninitialized storage, or after darken_reset()
// when the current population is intentionally being discarded.
//
// Walks the capacity-sized storage block once, handing each pool slot a
// permanent address. Only ->slot and ->owner are guaranteed to track an entity
// afterward; array position within a zone is not stable across moves.
static inline void darken_init(darken_t *ctx)
{
    for (uint16_t z = 0; z < ctx->zones; z++)
        ctx->bounds[z] = 0;

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

// 1.1-compatible: updates every user zone.
static inline void darken_update(darken_t *ctx)
{
    DARKEN_FOREACH(ctx, _DARKEN_UPDATE);
}

// Updates a single zone.
static inline void darken_update_zone(darken_t *ctx, uint16_t zone)
{
    DARKEN_FOREACH_ZONE(ctx, zone, _DARKEN_UPDATE);
}

// 1.1-compatible: destroys every active entity, then drops the pool back to the
// free zone.
//
// destroy() callbacks must not mutate the ctx's zones (delete / set_zone /
// spawn / reset) during the reset iteration.
static inline void darken_reset(darken_t *ctx)
{
    DARKEN_FOREACH(ctx, {
        if (_entity->destroy)
            _entity->destroy(_DARKEN_ARGS(_entity));
    });

    for (uint16_t z = 0; z < ctx->zones; z++)
        ctx->bounds[z] = 0;
}

// Zone-aware reset: destroys every active entity in `zone`, then moves them all
// back to the free zone.
//
// Entities in OTHER zones are not destroyed and their address/data pointer
// never changes (same guarantee as always -- see "Entity" section above), but
// their ->slot CAN change as a side effect: _darken_move_free() shrinks every
// zone boundary from the freed entity's own zone up through the last one, so
// entities in zones ABOVE `zone` may get physically repositioned within
// pool[] even though their zone membership and data are untouched (verified).
// Code should never rely on ->slot staying fixed across any zone-crossing
// operation, in this function or anywhere else in the engine.
static inline void darken_reset_zone(darken_t *ctx, uint16_t zone)
{
    uint16_t lo = _darken_zone_lo(ctx, zone);

    while (_darken_zone_hi(ctx, zone) > lo)
    {
        darken_entity_t _entity = ctx->pool[_darken_zone_hi(ctx, zone) - 1];

        if (_entity->destroy)
            _entity->destroy(_DARKEN_ARGS(_entity));

        _darken_move_free(_entity);
    }
}

/* ============================================================================
 * USAGE EXAMPLES
 * ============================================================================
 *
 * DYNAMIC (1.1-compatible):
 *     darken_t m = DARKEN_ALLOC(malloc, 5, sizeof(struct MyComponent));
 *     if (!m.pool || !m.storage) return;
 *     darken_init(&m);
 *     ...
 *     darken_reset(&m);
 *     DARKEN_FREE(free, &m);
 *
 * DYNAMIC with zones:
 *     darken_t m = DARKEN_ALLOC_ZONES(malloc, 5, 3, sizeof(struct MyComponent));
 *     if (!m.pool || !m.storage) return;
 *     darken_init(&m);
 *     ...
 *
 * STATIC (runtime binding, 1.1-compatible):
 *     static DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
 *     darken_t m = DARKEN_BIND(storage);
 *     darken_init(&m);
 *
 * STATIC (runtime binding, with zones):
 *     static DARKEN_DECLARE_ZONES(storage, 5, 3, sizeof(struct MyComponent));
 *     darken_t m = DARKEN_BIND(storage);
 *     darken_init(&m);
 *
 * STATIC (compile-time init):
 *     static DARKEN_DECLARE_ZONES(storage, 5, 3, sizeof(struct MyComponent));
 *     static darken_t m = DARKEN_INIT(storage);
 *
 *     void init_test_manager(void) {
 *         darken_init(&m);
 *         ...
 *     }
 *
 * ============================================================================
 * NOTES
 * ============================================================================
 *
 * - DARKEN_SPAWN special-cases zones == 1: instead of routing through
 *   _darken_move_from_free, it does bounds[0]++ directly. That recovers almost
 *   all of 1.1's spawn cost for single-zone contexts. Multi-zone ctxs pay the
 *   generic zone-movement cost, bounded by the number of zones.
 *
 * - darken_entity_delete special-cases zones == 1 the same way: a single swap
 *   against bounds[0]-1 instead of the generic scan + swap loop. Multi-zone
 *   ctxs still pay the generic cost.
 *
 * - darken_entity_set_zone is not O(1): it walks the zone bounds to reach the
 *   target zone. The cost is bounded by the number of zones and is paid only
 *   when an entity changes zone, not during a normal update of a zone.
 *   Adjacent-zone moves cost 1 swap regardless of how many zones the ctx has;
 *   far moves scale linearly with the distance.
 *
 * - DARKEN_FOREACH_ZONE() is safe for deleting / moving the current entity to
 *   FREE. Moving entities between user zones during the iteration can change the
 *   visited set because the zone boundaries move; callers that need strict
 *   one-pass semantics should not reorder entities between user zones from
 *   inside CODE.
 *
 * - darken_entity_delete destroys the entity regardless of which user zone it
 *   was in. Since 1.2 has no universal "paused" concept, every user zone is
 *   treated equally.
 *
 * - CROSS-ZONE SLOT MOVEMENT: no operation guarantees that entity->slot stays
 *   fixed across a zone crossing. A single-zone ctx is the only case where
 *   slot changes are confined to "one entity out, another in" without touching
 *   other entities' slots.
 */