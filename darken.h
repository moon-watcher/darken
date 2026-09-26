/**
 * darken.h — Darken (DARKula ENgine) Entity System
 *
 * darken-1.4.0_dev
 *
 * Full documentation: README.Darken.md
 *
 * ============================================================================
 * PORTABILITY / REQUIREMENTS -- READ THIS FIRST
 * ============================================================================
 *
 * Darken is a generic, target-agnostic entity manager: nothing in its logic depends on any specific
 * platform, engine, or toolchain. It operates over plain caller-provided storage and pointers, so it drops
 * into any C project -- from an 8-bit home computer to a 64-bit workstation. Darken itself has ZERO
 * #include lines of its own; everything below is built out of bare C keywords and macros. What's required
 * of whoever includes it:
 *
 * 1. Fixed-width integer types must already be visible BEFORE this header is included. Darken deliberately
 *    does NOT #include <stdint.h> itself -- the including project must provide them, whether via a plain
 *    `#include <stdint.h>` or whatever equivalent your target already defines them through.
 *
 * 2. Standard C89 as the language baseline, with one extension: C99 flexible array members (the trailing
 *    `data[]` member of struct darken_entity_t). Flexible array members have been part of ISO C since 1999,
 *    and were already a common pre-standard extension before that (cc65, the 6502 compiler used for
 *    machines like the C64, documents implementing them "as defined in the C99 ISO standard"; SDCC, used
 *    for Z80 targets like the Master System, tracks C99/C11/C23 fairly closely too).
 *
 *    `inline` is not available in strict C89, so every function in this header is declared through the
 *    DARKEN_INLINE macro, which resolves to `static inline` on any compiler advertising C99 or later, and
 *    to plain `static` otherwise. Both forms are valid C89 -- the C89 form just relies on the compiler to
 *    decide whether to actually inline. The user can predefine DARKEN_INLINE to override.
 *
 *    No compiler-specific attributes, no __alignof__, no C11 features, and (as of this version) no
 *    standard-library helper headers. Every alignment and offset computation is done with two small,
 *    self-contained tricks instead of the standard library's offsetof()/<stddef.h> or a compiler's
 *    __alignof__:
 *
 *      - Alignment (needed wherever a value has to be a genuine compile-time array size, as in
 *        DARKEN_DECLARE()) is computed with a plain sizeof() subtraction: for `struct { char c; TYPE t; }`,
 *        the padding the compiler must insert before `t` equals TYPE's alignment requirement, and a
 *        sizeof()-based expression like this is a true integer constant expression in every C standard since
 *        C89 -- unlike the trick below, which isn't guaranteed to be one.
 *      - The one place an actual byte *offset* is needed at runtime (DARKEN_ENTITY(), recovering an entity
 *        handle from a data pointer) uses the classic "address of a member through a null pointer" idiom --
 *        e.g. `&((struct darken_entity_t *)0)->data`. This is formally undefined behavior by a strict
 *        reading of the standard (no object of that type actually lives at address 0, so reaching `->MEMBER`
 *        through it is, technically, an access through an invalid pointer) -- but no read or write ever
 *        happens, only an address computation, and every C compiler in real-world use treats it exactly that
 *        way.
 *
 *    Both results are plain `unsigned long`, not size_t/uintptr_t -- or, for the alignment idiom, become one
 *    at the point they're used: `unsigned long` is a core-language type, not something a header hands you,
 *    and the C standard already guarantees it's at least 32 bits wide, which is more than enough for an
 *    entity's header+payload size on any target this library makes sense on. Concretely, _DARKEN_OFFSETOF's
 *    result is cast to `unsigned long` right in its own definition; _DARKEN_ALIGNOF's sizeof()-subtraction is
 *    left as whatever unsigned type sizeof() itself produces (never named anywhere), and picks up its
 *    `unsigned long` cast where the two are combined, in _DARKEN_ENTITY_STRIDE. Either way, nothing is ever
 *    declared as size_t or uintptr_t.
 *
 * 3. DARKEN_ALLOC(), DARKEN_BIND() and DARKEN_INIT() are written as C99 compound literals with designated
 *    initializers, purely for convenience. If your compiler only implements C89 and lacks those, nothing
 *    about the *engine* depends on them -- replace `darken_t m = DARKEN_ALLOC(...);` with a zero-initialized
 *    `darken_t m = {0};` followed by assigning `m.pool`, `m.storage`, `m.capacity`, `m.stride` by hand, using
 *    the same expressions the macro bodies use below.
 *
 * 4. CAPACITY must satisfy 1 <= CAPACITY <= max(darken_index_t), and the computed entity stride must fit in
 *    darken_index_t. These are API requirements, and DARKEN_DECLARE() enforces the easy half of them for
 *    free: CAPACITY == 0 is already a compile error (the pool array's size expression collapses to -1), simply
 *    as a side effect of how that array is declared, not a dedicated check. What DARKEN_DECLARE() can't check
 *    is the upper bound -- CAPACITY overflowing darken_index_t, or a PAYLOAD large enough that the resulting
 *    stride overflows it -- because a real range check would need a separate compile-time assertion, and
 *    that's a second declaration, which would break `static DARKEN_DECLARE(...)` (a storage-class specifier
 *    can only prefix one declaration). DARKEN_ALLOC() checks none of this, upper or lower bound, since its
 *    CAPACITY is an ordinary runtime expression, not something the preprocessor ever sees. Whichever bound
 *    goes unchecked, exceeding it silently truncates the stored capacity or stride at runtime rather than
 *    failing to compile.
 *
 * Beyond that, Darken makes no assumptions about the target: pointer width, struct alignment, and endianness
 * are all whatever the compiler says they are for the platform it's building for (see _DARKEN_ENTITY_ALIGN
 * and the null-pointer-offset macros around the storage declarations and stride calculation, rather than any
 * hardcoded value or compiler builtin).
 *
 * ONE genuinely non-portable corner is left, and it's opt-in: STATE-MACHINE mode's sentinel trick (see
 * below) round-trips small integers through a function-pointer type and orders them with `>`. That's well
 * defined in practice on any architecture with a single flat code/data address space -- i.e. essentially
 * every 8/16/32/64-bit CPU anyone actually targets (6502, Z80/8080, 68k, x86, ARM, RISC-V, MIPS, ...). It is
 * NOT guaranteed by the C standard, and it is NOT safe on genuine Harvard-architecture micros where code and
 * data live in separate address spaces with differently-shaped pointers (classic AVR, some PIC cores, and
 * similar). If that's your target, define DARKEN_DIRECT instead: direct mode never converts an integer to a
 * function pointer, so this whole concern disappears.
 *
 *
 * ============================================================================
 * CONFIGURATION -- all opt-in, all optional, all defaulted
 * ============================================================================
 *
 * The defaults below are tuned for 32-bit and 64-bit targets: no artificial limits, all features present,
 * code reads clean. On 8-bit and 16-bit machines the size and speed knobs below trade flexibility for bytes
 * and cycles, and no single knob is required -- each one stands on its own, and any combination is legal.
 *
 *   DARKEN_SMALL
 *       Convenience preset for 8-bit targets. Expands, ONLY for the macros that aren't already defined, to:
 *           DARKEN_INDEX_T = uint8_t   (pool capped at 255 entities)
 *           DARKEN_USR_T   = uint8_t   (usr field is 1 byte)
 *       It does NOT remove any field and does NOT change any behaviour. The size knobs that would drop
 *       fields (DARKEN_NO_TAG, DARKEN_NO_USR, DARKEN_NO_DESTROY) and the mode switch (DARKEN_DIRECT) stay
 *       explicit opt-ins, because each is a semantic change that can break existing code. Any individual
 *       macro you define yourself wins over the preset.
 *
 *   DARKEN_INDEX_T       default uint16_t
 *       Type of every internal index, capacity, size and stride. On 8-bit targets, switch to uint8_t to cap
 *       the pool at 255 and save 1-2 bytes per entity header. The reason to switch is memory, not speed:
 *       on Z80 a uint8_t index can be a couple of cycles *slower* per foreach iteration than uint16_t, due
 *       to the extra zero-extension the compiler inserts before the multiply-by-2 that indexes the pointer
 *       array (which is a 2-byte element on 8-bit targets, so the arithmetic is 16-bit either way). The
 *       real per-iteration savings live in the loop counter's decrement/test, and they are small.
 *
 *   DARKEN_USR_T         default uint16_t
 *       Type of the user-defined `usr` field. Drop to uint8_t on targets that never need more than 255
 *       distinct values.
 *
 *   DARKEN_TAG_T         default uint32_t
 *       Type of the user-defined `tag` field. `tag` is the single fattest member of the header on 8-bit
 *       targets; dropping it to uint16_t or uint8_t, or removing it entirely via DARKEN_NO_TAG, is the
 *       best single memory win available on those machines.
 *
 *   DARKEN_NO_TAG
 *       Removes `tag` from the entity header entirely. Any code that then touches entity->tag fails to
 *       compile, which is the intended feedback.
 *
 *   DARKEN_NO_USR
 *       Removes `usr` from the entity header entirely. Same compile-time-feedback rule as DARKEN_NO_TAG.
 *
 *   DARKEN_NO_DESTROY
 *       Removes the `destroy` callback field from the entity header, and the destroy-invocation paths from
 *       darken_entity_delete(), darken_update() and darken_reset(). Saves one function pointer per entity
 *       (2 bytes on 8-bit, 4 on 32-bit, 8 on 64-bit). This is a semantic change: without it, an entity has
 *       no automatic cleanup hook, and DARKEN_DELETE (or a manual darken_entity_delete) simply removes the
 *       entity. Any code that assigns entity->destroy fails to compile.
 *
 *   DARKEN_MIGRATE_WORD_T   default unsigned char
 *       Word size used to bulk-copy entity bytes in darken_entity_migrate(). Defaults to unsigned char,
 *       which is both correct on every target and one of the three character types the strict-aliasing
 *       rule (C99 6.5p7) exempts by name -- unlike uint8_t, which almost always *is* unsigned char under
 *       the hood but isn't required to be by the standard's text, so it isn't what 6.5p7 actually names.
 *       Because the default is a genuine character type, the byte-copy loop it generates is never
 *       undefined behavior. Modern compilers (GCC, Clang, MSVC) recognise the byte-copy loop and vectorise
 *       it into a wide memcpy when it pays off, so leaving the default costs nothing on 32/64-bit. If you
 *       override this to a wider type for
 *       a specific reason -- typically on an 8-bit toolchain where the byte loop really is the fastest
 *       thing -- be aware that the copy then goes through a pointer that is NOT a character type, and is
 *       therefore technically undefined behavior under the C strict-aliasing rule. It works in practice on
 *       every toolchain whose optimizer does not exploit strict aliasing (SDCC, cc65, and any build with
 *       -fno-strict-aliasing). If you widen it, also verify that the wider type's alignment does not exceed
 *       the entity's own (see the ALIGNMENT CAVEAT below): on targets where it does, the wide accesses
 *       would be misaligned even if the aliasing rule didn't apply.
 *
 *   DARKEN_DIRECT
 *       Switches the engine from state-machine mode (default) to direct-callback mode. See "Update /
 *       lifecycle control" below. Also the mode to prefer on Harvard-architecture micros, where the
 *       default mode's function-pointer sentinels are unsafe.
 *
 * ----------------------------------------------------------------------------
 * ALIGNMENT CAVEAT for DARKEN_INDEX_T / DARKEN_USR_T / DARKEN_TAG_T
 * ----------------------------------------------------------------------------
 *
 * Each of these fields is placed in the entity header BEFORE the flexible array member `data[]`, and the
 * whole design relies on `data[]` landing at an offset that is a multiple of the struct's own alignment.
 * That is guaranteed by the C layout rules as long as no field in the header requires STRICTER alignment
 * than a pointer. On every mainstream ABI (8-bit, 16-bit, 68K, 32-bit x86/ARM/MIPS, 64-bit) this holds for
 * any integer type up to the pointer's width.
 *
 * ONE real-world ABI breaks it: ARM32 with the EABI, where uint64_t is 8-aligned but void* is 4-aligned.
 * Using uint64_t (or any 8-aligned type) for DARKEN_INDEX_T, DARKEN_USR_T or DARKEN_TAG_T there will
 * misalign `data[]` for 8-aligned payloads, and a misaligned LDRD/STRD on ARM can fault or corrupt data.
 * Don't do it. uint32_t and below are always safe. All preset and default combinations respect this.
 *
 *
 * ============================================================================
 * Entity: Base entity managed by the entity ctx
 * ============================================================================
 *
 * The entity structure serves as a container for user data with lifecycle management. The flexible array
 * member 'data[]' allows entities to have variable-sized payloads while maintaining contiguous memory layout.
 *
 * The stride between entities is pre-calculated once, at ctx initialization, purely to lay out the fixed
 * storage block (darken_init() walks it in `stride`-sized steps to hand each entity its permanent address).
 * O(1) access by index comes from `pool[]` itself being a flat array of pointers -- stride plays no part in
 * any lookup after init.
 *
 * An entity's own memory address (this struct) never moves once allocated by darken_init(). What moves between
 * the ctx's zones is only the *pointer* to it inside darken.pool[]. This is what makes it safe to keep a raw
 * pointer into entity->data even while the entity gets reordered.
 *
 * The ctx itself needs the same guarantee, for the same reason: darken_init() bakes the address it was given
 * into every entity's ->owner, so the darken_t instance must already be sitting at its final address before
 * darken_init() runs, and must stay there for as long as the ctx is used. A helper that builds a darken_t on
 * its own stack and returns it *by value* will compile and look fine, but leaves every entity's ->owner
 * dangling into a stack frame that no longer exists. Build it in place, or pass a `darken_t *` in.
 *
 *
 * ============================================================================
 * Ctx: Entity container and lifecycle ctx.
 * ============================================================================
 *
 * Maintains the pointer array in two logical zones:
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
 * update/destroy/tag/usr. An entity handed out by DARKEN_SPAWN() -- whether fresh from darken_init() or
 * recycled after a previous entity in that slot was deleted -- may still carry whatever values that slot's
 * previous occupant left behind. Setting these fields to the values your entity actually needs (including
 * clearing any you don't want carried over) is the caller's responsibility on every spawn.
 *
 * ----------------------------------------------------------------------------
 * WARNING -- update == NULL is a hard crash, not a no-op
 * ----------------------------------------------------------------------------
 *
 * darken_update() calls every active entity's ->update unconditionally. If you spawn an entity and do not
 * assign ->update before the next darken_update() call, the engine will call through a NULL function
 * pointer. On hosted targets this is a segfault; on an 8-bit micro it is whatever the CPU does when it
 * jumps to address 0 -- on Z80 that is RST 0, which resets the machine. There is no NULL check in the
 * engine: the branch would add cycles to every entity on every frame, and the design chooses to trust the
 * caller instead. Always assign ->update (and ->destroy, and any field you rely on) immediately after
 * every DARKEN_SPAWN().
 *
 *
 * ============================================================================
 * Update / lifecycle control -- two selectable modes
 * ============================================================================
 *
 * STATE-MACHINE mode is the default. Define DARKEN_DIRECT before including this header to opt into direct
 * mode instead (also the mode to prefer if you're targeting a Harvard-architecture micro -- see the
 * portability note above).
 *
 * Each mode has one fixed callback signature -- there is no separate configuration macro for the argument
 * list. The signature is chosen per mode to match how that mode is actually used: state-machine callbacks
 * rarely need the entity handle, since the return value drives the lifecycle; direct-mode callbacks almost
 * always need it, since they call darken_entity_delete() themselves.
 *
 * 1) STATE-MACHINE mode -- default
 * ---------------------------------------------------------
 *     Signature: darken_state_t callback(void *data)
 *
 *     Only the entity's payload is passed -- never the entity handle. darken_update() reads update()'s
 *     return value and drives the lifecycle itself:
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
 *     Its return value is always ignored -- darken_reset() and darken_entity_delete() only ever call it
 *     for its side effects. (Omitted entirely when DARKEN_NO_DESTROY is defined.)
 *
 *     `destroy` must not mutate the ctx's pool zones. Deleting, spawning, or otherwise reordering entities
 *     from inside a destroy callback will corrupt the swap state and iteration that the engine relies on.
 *     This restriction applies to every path that invokes destroy: darken_reset(), darken_entity_delete(),
 *     and the DARKEN_DELETE branch inside darken_update().
 *
 *     Need the entity handle anyway (e.g. to read/write usr or tag)? Recover it with DARKEN_ENTITY(data).
 *
 *     Comparing a darken_state_t value against the sentinels with `==`/`>` relies on your target having a
 *     single flat address space, so that a function pointer is, in effect, just an address (see the
 *     PORTABILITY note at the top of this file). On a Harvard-architecture target, use DARKEN_DIRECT mode
 *     instead, which never performs this conversion.
 *
 * 2) DIRECT mode -- DARKEN_DIRECT defined
 * ---------------------------------------------------------
 *     Signature: void callback(darken_entity_t entity)
 *                void callback(darken_entity_t entity, void *data)
 *
 *     Both the entity handle and its payload are passed, in that order. darken_update() just calls
 *     entity->update(entity, entity->data) every frame and ignores any return value; entity->destroy(entity,
 *     entity->data) is called the same way by darken_reset() and darken_entity_delete(). The callback is in
 *     full control of the entity's lifecycle: it changes state by assigning directly to entity->update
 *     (and/or entity->destroy), and it deletes itself by calling darken_entity_delete() -- which takes the
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
 *     the rest are pushed and ignored. This is a common-and-widely-supported (but not ISO-C-guaranteed)
 *     calling-convention convenience; if your target's ABI doesn't tolerate mismatched argument counts on an
 *     unprototyped function pointer, always declare both parameters.
 */

#ifndef DARKEN_H
#define DARKEN_H

// Darken does NOT #include anything -- not <stdint.h>, not <stddef.h>, nothing. See "PORTABILITY /
// REQUIREMENTS" at the top of this file. Make sure uint8_t/uint16_t/uint32_t are visible before this point:
// either `#include <stdint.h>` yourself, or rely on whatever your platform already provides in its place.
// Everything else Darken needs (offsets, alignment) is built from bare `unsigned long` and the null-pointer
// member-address idiom, both of which are core language, not library, features.

/* ============================================================================
 * CONFIGURATION
 * ============================================================================ */

// DARKEN_INLINE -- resolves to `static inline` on C99+ and to plain `static` on C89. Either form is valid
// C89; the difference is only whether the compiler is required to at least try to inline the function.
// Predefine DARKEN_INLINE yourself to override.
#ifndef DARKEN_INLINE
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define DARKEN_INLINE static inline
#else
#define DARKEN_INLINE static
#endif
#endif

// DARKEN_SMALL: single-switch preset for 8-bit targets. Expands only into macros that aren't already
// defined, so any individual override you set yourself wins. It changes ONLY types -- no field is removed,
// no behaviour changes -- so any code that compiles against the defaults still compiles against DARKEN_SMALL,
// just with a smaller per-entity footprint. The field-removal knobs (DARKEN_NO_TAG, DARKEN_NO_USR,
// DARKEN_NO_DESTROY) and the mode switch (DARKEN_DIRECT) stay explicit opt-ins because each one is a
// semantic change that can break existing code.
#ifdef DARKEN_SMALL
#ifndef DARKEN_INDEX_T
#define DARKEN_INDEX_T uint8_t
#endif
#ifndef DARKEN_USR_T
#define DARKEN_USR_T uint8_t
#endif
#endif

// darken_index_t -- internal index/capacity/size/stride type. Default uint16_t.
#ifndef DARKEN_INDEX_T
#define DARKEN_INDEX_T uint16_t
#endif
typedef DARKEN_INDEX_T darken_index_t;

// usr -- user-defined field.
#ifdef DARKEN_NO_USR
#define _DARKEN_USR_DECL
#else
#ifndef DARKEN_USR_T
#define DARKEN_USR_T uint16_t
#endif
#define _DARKEN_USR_DECL DARKEN_USR_T usr;
#endif

// tag -- user-defined identification/categorization field.
#ifdef DARKEN_NO_TAG
#define _DARKEN_TAG_DECL
#else
#ifndef DARKEN_TAG_T
#define DARKEN_TAG_T uint32_t
#endif
#define _DARKEN_TAG_DECL DARKEN_TAG_T tag;
#endif

// destroy -- optional cleanup callback.
#ifdef DARKEN_NO_DESTROY
#define _DARKEN_DESTROY_DECL
#else
#define _DARKEN_DESTROY_DECL darken_state_t destroy;
#endif

// Callback type. Return type differs per mode.
#ifdef DARKEN_DIRECT
typedef void (*darken_state_t)();
#else
typedef void *(*darken_state_t)();
#endif

typedef struct darken_entity_t *darken_entity_t;

typedef struct darken_t
{
    darken_entity_t *pool;  // Pointer array to entities in the ctx's storage block
    unsigned char *storage; // Pointer to the contiguous memory block where entities are allocated
    darken_index_t capacity;
    darken_index_t size;
    darken_index_t stride;
} darken_t;

// _DARKEN_USR_DECL / _DARKEN_DESTROY_DECL / _DARKEN_TAG_DECL each already include their own trailing `;`
// when the field they guard is present, and expand to *nothing at all* -- not even a stray `;` -- when it
// isn't (via the corresponding DARKEN_NO_* macro). That's why none of the three is followed by a `;` at
// either use site below (here and in struct _darken_hdr_shape_t): adding one would either double up into
// `;;` when the field is present, or stand alone as an empty declaration when it's absent -- both are
// extensions some compilers only tolerate with a warning, and neither is guaranteed by the standard.
struct darken_entity_t
{
    darken_index_t slot;   // Private: Index in the ctx's pool array
    _DARKEN_USR_DECL       // User-defined field for custom data
    darken_state_t update; // User-defined update callback
    _DARKEN_DESTROY_DECL   // User-defined destroy callback
    _DARKEN_TAG_DECL       // User-defined tag for identification or categorization
    darken_t *owner;       // Private: Pointer to the owning ctx
    unsigned char data[];  // Payload
};

/* ============================================================================
 * PRIVATE
 * ============================================================================ */

// Mirrors every fixed (non-flexible-array) member of struct darken_entity_t, but has no flexible array
// member of its own. It exists purely so the rest of this header has *something* legal to put inside a
// union or another struct: ISO C forbids embedding a struct that (recursively) contains a flexible array
// member inside another struct, union, or array (C11 6.7.2.1p3) -- struct darken_entity_t itself is off
// limits for that. Nothing is ever stored through this type; only its size and alignment are used. Keeping
// the member list here identical to darken_entity_t's fixed members (including which are present/absent
// under the DARKEN_NO_* macros) is what guarantees the alignment calculation stays correct for any
// configuration: if you add a member to darken_entity_t, mirror it here too.
struct _darken_hdr_shape_t
{
    darken_index_t slot;
    _DARKEN_USR_DECL
    darken_state_t update;
    _DARKEN_DESTROY_DECL
    _DARKEN_TAG_DECL
    darken_t *owner;
};

// The classic "address of a member through a null pointer" idiom, used instead of the standard library's
// offsetof() so this header can avoid #including <stddef.h>. Formally undefined behavior by a strict reading
// of the standard; no object at address 0 is ever read or written, only its address is computed, which is
// why every real-world C compiler treats this as well-defined in practice. Only used at runtime
// (DARKEN_ENTITY() below) -- a pointer-to-integer cast like this one is NOT guaranteed to be a compile-time
// integer constant expression, so it's no good for sizing an array, which is exactly why alignment (needed
// for that) is computed a different way, just below.
#define _DARKEN_OFFSETOF(TYPE, MEMBER) ((unsigned long)&((TYPE *)0)->MEMBER)

// Portable, compile-time-constant alignment computation using nothing but sizeof(). For any complete object
// type, the standard guarantees sizeof(TYPE) is itself a multiple of TYPE's own alignment -- that's exactly
// what makes arrays of TYPE work. So in `struct { char c; TYPE t; }`, the padding a compiler must insert
// before `t` to satisfy TYPE's alignment -- and only that padding -- accounts for the entire difference
// between the wrapper's size and TYPE's own size; there's no leftover trailing padding to subtract out,
// because TYPE's size was already a multiple of its alignment. That difference is exactly alignof(TYPE).
// Unlike the null-pointer idiom above, a plain sizeof() subtraction like this one IS a genuine integer
// constant expression in every C standard since C89, which is required here: DARKEN_DECLARE() needs a real
// compile-time array size, not just a runtime-correct value. TYPE must not (recursively) contain a flexible
// array member -- see _darken_hdr_shape_t above for why struct darken_entity_t can't be passed here directly.
#define _DARKEN_ALIGNOF(TYPE) (sizeof(struct { char _darken_c; TYPE _darken_m; }) - sizeof(TYPE))

// The real alignment requirement of struct darken_entity_t, obtained via its FAM-free stand-in above.
#define _DARKEN_ENTITY_ALIGN _DARKEN_ALIGNOF(struct _darken_hdr_shape_t)

// Single call-site helper for invoking update()/destroy(), used everywhere the engine calls into user code.
// The argument list is fixed per mode (see the big comment above), so there is nothing to configure here.
#ifdef DARKEN_DIRECT
#define _DARKEN_ARGS(ENTITY) (ENTITY), (ENTITY)->data
#else
#define _DARKEN_ARGS(ENTITY) (ENTITY)->data
#endif

// Conditionally invokes destroy(), or compiles away to nothing when DARKEN_NO_DESTROY is defined. Defined
// once, here, at file scope, rather than wrapping each call site in #ifndef DARKEN_NO_DESTROY / #endif --
// darken_update() and darken_reset() need to make that call from *inside* the block of code they hand to
// DARKEN_FOREACH(), and a raw preprocessor directive inside a macro's argument list is explicitly undefined
// behavior (C99/C11 6.10.3p11). GCC happens to expand it as expected anyway (with a warning), but a
// simpler or stricter preprocessor -- exactly the kind more likely on an 8-bit toolchain -- is not obliged
// to. Routing every call through this one macro means no #ifndef/#endif pair ever needs to sit inside a
// macro argument again. The `(void)(ENTITY)` in the disabled branch isn't a no-op for its own sake -- it
// keeps ENTITY "used" so callers don't get an unused-variable warning when DARKEN_NO_DESTROY removes the
// only thing that would otherwise reference it (see darken_reset()).
#ifdef DARKEN_NO_DESTROY
#define _DARKEN_MAYBE_DESTROY(ENTITY) ((void)(ENTITY))
#else
#define _DARKEN_MAYBE_DESTROY(ENTITY) \
    if ((ENTITY)->destroy)            \
        (ENTITY)->destroy(_DARKEN_ARGS(ENTITY));
#endif

// Rounds (header + payload) up to the next multiple of the entity's required alignment. Pure `unsigned long`
// arithmetic -- no uintptr_t, no size_t, no __alignof__, no compiler builtins, no headers. Assumes the
// alignment is a power of two, which the C standard doesn't strictly mandate but which is true of every
// alignment value on every mainstream ABI, 8-bit through 64-bit.
#define _DARKEN_ENTITY_STRIDE(PAYLOAD) (((sizeof(struct darken_entity_t) + (PAYLOAD)) + (unsigned long)_DARKEN_ENTITY_ALIGN - 1) & ~((unsigned long)_DARKEN_ENTITY_ALIGN - 1))

/* ============================================================================
 * PUBLIC API
 * ============================================================================ */

#ifndef DARKEN_DIRECT
// Sentinel return values for update() callbacks in state-machine mode.
// Any value > DARKEN_CONTINUE is treated as the next update callback.
#define DARKEN_CONTINUE ((darken_state_t)1)
#define DARKEN_DELETE ((darken_state_t)0)
#endif

// Word size used to bulk-copy an entity's bytes in darken_entity_migrate(). The default is unsigned char --
// not uint8_t, even though the two are the same type on every real compiler -- because unsigned char is one
// of the three types the strict-aliasing rule (C99 6.5p7) names explicitly, while uint8_t is only guaranteed
// to be *some* unsigned 8-bit type, not necessarily unsigned char by the standard's text. Widening it
// (to uint16_t/uint32_t/uint64_t) produces code that goes through a non-character pointer and is formally
// UB under strict aliasing; see the CONFIGURATION section at the top of this file for the full story.
// Where the widening is safe, it's a speed knob only -- correctness never depends on it.
#ifndef DARKEN_MIGRATE_WORD_T
#define DARKEN_MIGRATE_WORD_T unsigned char
#endif

// Dynamic allocation: use with malloc/calloc or custom allocator
//     darken_t m = DARKEN_ALLOC(malloc, 5, sizeof(struct MyComponent));
//     if (!m.pool || !m.storage) return;
//     darken_init(&m);
//     ...
//     DARKEN_FREE(free, &m);
//
// DARKEN_ALLOC() does not handle allocation failure or partial allocation cleanup.
// CAPACITY must satisfy 1 <= CAPACITY <= max(darken_index_t), and the computed stride must fit in
// darken_index_t. Unlike DARKEN_DECLARE(), DARKEN_ALLOC() performs no compile-time validation (its
// CAPACITY is a runtime expression); the caller is responsible for providing valid values. A CAPACITY or
// stride that exceeds darken_index_t silently truncates at runtime.
//
// ALLOC is trusted to hand back memory aligned suitably for any object type -- guaranteed for malloc()/
// calloc() on a hosted implementation, but NOT guaranteed for a hand-rolled pool allocator on a freestanding
// 8-bit target. If ALLOC is a custom allocator, make sure it returns at least _DARKEN_ENTITY_ALIGN-aligned
// memory, or use DARKEN_DECLARE()'s static storage below instead, which this header aligns for you.
#define DARKEN_ALLOC(ALLOC, CAPACITY, PAYLOAD)                           \
    (darken_t)                                                           \
    {                                                                    \
        .pool = (ALLOC)((CAPACITY) * sizeof(darken_entity_t)),           \
        .storage = (ALLOC)((CAPACITY) * _DARKEN_ENTITY_STRIDE(PAYLOAD)), \
        .capacity = (darken_index_t)(CAPACITY),                          \
        .stride = (darken_index_t)_DARKEN_ENTITY_STRIDE(PAYLOAD),        \
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
//     static DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
//     darken_t m = DARKEN_BIND(storage);
//     darken_init(&m);
//
// CAPACITY must satisfy 1 <= CAPACITY <= max(darken_index_t), and the computed stride must fit in
// darken_index_t. These are API requirements; the caller is responsible for keeping both values in range.
// A CAPACITY that doesn't fit in darken_index_t, or a PAYLOAD large enough that the resulting stride
// doesn't fit, silently truncates at runtime.
//
// The payload type used with DARKEN_DATA() must not require stricter alignment than struct darken_entity_t.
//
// The byte block sits inside a union alongside a (never-instantiated) _darken_hdr_shape_t member, purely so
// the compiler is forced to give the block itself at least _DARKEN_ENTITY_ALIGN alignment -- ordinary,
// portable C union layout rules guarantee this, with no compiler-specific attribute involved. Only the
// block's first byte needs to land on that alignment: every entity after the first sits at a `stride`-sized
// (a multiple of the alignment) offset from it, so the whole block stays correctly aligned throughout.
//
// Expands to a single declaration, so it can be prefixed with `static`, used at file scope, or nested inside
// a larger declaration if the caller wants to -- no typedefs, no helper statements, nothing that would break
// one-declaration-only contexts.
//
// NOTE for anyone who was reaching into a DARKEN_DECLARE()'d object's fields directly instead of going
// through DARKEN_BIND()/DARKEN_INIT(): the raw byte block used to be the top-level field `.data`; it is now
// `.block.bytes`, for the reason above.
#define DARKEN_DECLARE(NAME, CAPACITY, PAYLOAD)                               \
    struct                                                                    \
    {                                                                         \
        darken_index_t capacity;                                              \
        darken_index_t stride;                                                \
        darken_entity_t pool[(CAPACITY) ? (CAPACITY) : -1];                   \
        union                                                                 \
        {                                                                     \
            struct _darken_hdr_shape_t _darken_align;                         \
            unsigned char bytes[(CAPACITY) * _DARKEN_ENTITY_STRIDE(PAYLOAD)]; \
        } block;                                                              \
    } NAME = {                                                                \
        .capacity = (darken_index_t)(CAPACITY),                               \
        .stride = (darken_index_t)_DARKEN_ENTITY_STRIDE(PAYLOAD),             \
    }

// Static/global initialization: compile-time constants.
// NOTE: DARKEN_INIT expands to a compound literal, which is not a constant expression in strict C99 -- it
// requires C11 (or the GNU C extension) to initialize an object with static storage duration. See item 3 of
// the PORTABILITY note at the top of this file for the plain-assignment fallback on C89-only compilers.
#define DARKEN_INIT(STORAGE)                                                                                            \
    (darken_t)                                                                                                          \
    {                                                                                                                   \
        .pool = (STORAGE).pool,                                                                                         \
        .storage = (STORAGE).block.bytes,                                                                               \
        .capacity = (darken_index_t)(sizeof((STORAGE).pool) / sizeof(darken_entity_t)),                                 \
        .stride = (darken_index_t)(sizeof((STORAGE).block.bytes) / (sizeof((STORAGE).pool) / sizeof(darken_entity_t))), \
    }

// Runtime binding: locals, reassignment, any context.
#define DARKEN_BIND(NAME)              \
    (darken_t)                         \
    {                                  \
        .pool = (NAME).pool,           \
        .storage = (NAME).block.bytes, \
        .capacity = (NAME).capacity,   \
        .stride = (NAME).stride,       \
    }

// Spawn a new entity from the free zone. Returns the entity or 0 if no free slots.
//
// The returned entity may contain garbage from a previous occupant -- always initialize all fields you care
// about (update, destroy, tag, usr, and data). In particular, always assign ->update before the next
// darken_update() call, or the engine will call through a NULL function pointer and the target will do
// whatever it does on a jump to address 0 (segfault on hosted targets; RST 0 / machine reset on Z80).
#define DARKEN_SPAWN(CTX) ((CTX)->size < (CTX)->capacity ? (CTX)->pool[(CTX)->size++] : 0)

// Iterate over all active entities in REVERSE order (from size-1 down to 0).
//
// The index-based form is deliberate. A pointer-walking variant was tried and measured on Z80: when CODE
// is a plain expression that doesn't clobber HL/DE, pointer walking wins by about 13%, but when CODE is a
// function call -- which is what darken_update() always does -- the callee is free to clobber those
// registers, so the loop state has to be spilled to memory every iteration anyway, and the two forms
// become cycle-for-cycle equivalent. The index form reserves fewer registers for the loop, and reads as
// the canonical C idiom. On 32/64-bit compilers the two generate identical code, so there is nothing to
// gain by changing it there either.
//
// Deleting the currently visited entity from inside CODE is safe and cheap: it swaps with the last active
// slot, which the loop has already passed.
//
// Deleting a *different* entity from inside CODE is subtler. If the deleted entity's slot is greater than
// the current loop index, the swap moves a slot the loop already visited into the now-empty position, and
// that entity is NOT revisited this frame -- correct. If the deleted entity's slot is LOWER than the
// current loop index, the entity that was at size-1 gets moved into a position the loop hasn't reached
// yet, and will be visited a second time this same frame. That is not a bug, but it can double-invoke an
// update callback within one frame for the moved entity. Callbacks that are idempotent per frame are
// unaffected; callbacks that advance internal timers will see that entity advance twice.
#define DARKEN_FOREACH(CTX, CODE)                    \
    do                                               \
    {                                                \
        darken_index_t _index = (CTX)->size;         \
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

// Recover the entity handle from a pointer to its data payload (mostly useful in STATE-MACHINE mode, where
// callbacks only receive data).
// DATA must point to the beginning of an entity's data[] payload.
// Uses the null-pointer member-address idiom described at the top of this file (_DARKEN_OFFSETOF), rather
// than the standard library's offsetof(), so this header stays dependency-free.
#define DARKEN_ENTITY(DATA) ((darken_entity_t)((unsigned char *)(DATA) - _DARKEN_OFFSETOF(struct darken_entity_t, data)))

// Zone membership tests.
// Note: ENTITY is evaluated multiple times per test. Do not pass expressions with side effects.
#define DARKEN_ENTITY_IS_ACTIVE(ENTITY) ((ENTITY)->slot < (ENTITY)->owner->size)
#define DARKEN_ENTITY_IS_FREE(ENTITY) (!DARKEN_ENTITY_IS_ACTIVE(ENTITY))

// Zone sizes, so callers don't have to do the size/capacity math by hand.
#define DARKEN_COUNT_ACTIVE(CTX) ((CTX)->size)
#define DARKEN_COUNT_FREE(CTX) ((darken_index_t)((CTX)->capacity - (CTX)->size))

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
DARKEN_INLINE void darken_entity_swap(darken_entity_t e1, darken_entity_t e2)
{
    if (e1 == e2)
        return;

    darken_t *ctx1 = e1->owner;
    darken_t *ctx2 = e2->owner;
    darken_index_t i = e1->slot;
    darken_index_t j = e2->slot;

    ctx1->pool[i] = e2;
    ctx2->pool[j] = e1;
    e1->slot = j;
    e2->slot = i;
    e1->owner = ctx2;
    e2->owner = ctx1;
}

// Delete an active entity, calling destroy() first if one is set and DARKEN_NO_DESTROY is not defined.
// destroy() must not mutate the ctx's pool zones (delete/spawn) while it runs -- see the big header comment.
DARKEN_INLINE void darken_entity_delete(darken_entity_t entity)
{
    if (DARKEN_ENTITY_IS_FREE(entity))
        return;

    _DARKEN_MAYBE_DESTROY(entity);
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
// The copy runs in DARKEN_MIGRATE_WORD_T-sized chunks with a byte tail for the remainder, using division and
// modulo against sizeof(DARKEN_MIGRATE_WORD_T) rather than a hardcoded shift/mask, so it stays correct for
// any word size you configure it to (1, 2, 4, 8, ...). The default word type is unsigned char, a genuine
// character type per the standard's own definition and therefore always safe under the strict-aliasing
// rule; see the CONFIGURATION section at the top of this file for the caveat that applies if you widen it.
//
// The wide accesses (when you widen the word type) are safe from an alignment standpoint without any runtime
// check on any target: entities are laid out at storage + i * stride, where storage is aligned to
// _DARKEN_ENTITY_ALIGN (see the union in DARKEN_DECLARE()) and stride is a multiple of it. If you widen
// DARKEN_MIGRATE_WORD_T, make sure its alignment does not exceed _DARKEN_ENTITY_ALIGN on your target (see
// the ALIGNMENT CAVEAT in the CONFIGURATION section). On byte-addressable CPUs with no hardware alignment
// fault at all (which covers essentially every 8-bit micro), alignment was never a correctness concern to
// begin with.
DARKEN_INLINE darken_entity_t darken_entity_migrate(darken_entity_t entity, darken_t *dst)
{
    darken_t *src = entity->owner;

    if (src == dst || dst->size >= dst->capacity)
        return 0;

    darken_index_t count = src->stride < dst->stride ? src->stride : dst->stride;
    darken_index_t words = (darken_index_t)(count / sizeof(DARKEN_MIGRATE_WORD_T));
    darken_index_t dst_slot = dst->size++;
    darken_entity_t moved = dst->pool[dst_slot];
    DARKEN_MIGRATE_WORD_T *d = (DARKEN_MIGRATE_WORD_T *)moved;
    DARKEN_MIGRATE_WORD_T *s = (DARKEN_MIGRATE_WORD_T *)entity;

    while (words--)
        *d++ = *s++;

    unsigned char *sb = (unsigned char *)s;
    unsigned char *db = (unsigned char *)d;

    count = (darken_index_t)(count % sizeof(DARKEN_MIGRATE_WORD_T));
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
//     DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
//     darken_t m = DARKEN_BIND(storage);
//     darken_init(&m);
//
// STATIC (file-scope, internal linkage):
//     static DARKEN_DECLARE(storage, 5, sizeof(struct MyComponent));
//     static darken_t world = DARKEN_INIT(storage);
//
//     void init_world() {
//         darken_init(&world);
//         ...
//     }
//
// 8-BIT TARGET (Z80/6502) with the DARKEN_SMALL preset:
//     #define DARKEN_SMALL
//     #include "darken.h"
//     static DARKEN_DECLARE(storage, 32, sizeof(struct bullet));
//     static darken_t world = DARKEN_INIT(storage);
//
//     Per-entity header with DARKEN_SMALL on a byte-addressed target (all fields still present):
//       slot (1) + usr (1) + update (2) + destroy (2) + tag (4) + owner (2) = 12 bytes.
//
//     If you additionally want to drop fields for further savings, add any of DARKEN_NO_TAG,
//     DARKEN_NO_USR or DARKEN_NO_DESTROY as separate defines -- each one is a semantic change and will
//     break any code that touches the removed field, which is the intended feedback.
//
// darken_init() must only be called on unused/uninitialized storage, or after darken_reset() when the
// current population is intentionally being discarded. It does not call destroy() for the entities already
// managed by the ctx.
//
// Walks the capacity-sized storage block once, handing each pool slot a permanent address. The loop counts
// down (capacity-1 to 0) rather than up; the direction itself is not significant and does not imply slot i
// lives at storage offset i*stride. Only ->slot and ->owner, not array position, are guaranteed to track an
// entity afterward.
DARKEN_INLINE void darken_init(darken_t *ctx)
{
    ctx->size = 0;
    darken_index_t i = ctx->capacity;
    unsigned char *storage = ctx->storage;

    while (i--)
    {
        ctx->pool[i] = (darken_entity_t)storage;
        ctx->pool[i]->owner = ctx;
        ctx->pool[i]->slot = i;

        storage += ctx->stride;
    }
}

DARKEN_INLINE void darken_update(darken_t *ctx)
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

        _DARKEN_MAYBE_DESTROY(_entity);
        darken_entity_swap(_entity, ctx->pool[--ctx->size]);
    });
#endif
}

// Calls destroy() on every currently active entity, then drops the whole pool back to the free zone
// (size = 0).
//
// This does NOT re-run darken_init(), so ->slot, ->owner, and the pool's mapping to storage are left as
// they were. Entities created after the reset start from the same free-zone position as before and are
// spawned with whatever garbage their slot's previous occupant left behind -- reassign every field you
// care about on each spawn, exactly as at cold boot.
//
// destroy() callbacks used by darken_reset() must not mutate the ctx's pool zones by deleting, spawning,
// or otherwise reordering entities during the reset iteration. (This is a specific case of the general
// restriction documented in the big header comment above.)
//
// With DARKEN_NO_DESTROY defined, the per-entity loop body becomes a no-op ((void)-cast, no side effects),
// so an optimizing compiler is free to delete the whole traversal and leave just the `size = 0` assignment
// -- confirmed with objdump: GCC does exactly that at -O2, but at -O0 the full loop (compare, branch,
// pointer load) is still there in the generated code, doing nothing. This is an optimization a compiler MAY
// perform, not a guarantee of the language, so don't rely on it for cycle counts on a target whose compiler
// doesn't do this kind of dead-code elimination.
DARKEN_INLINE void darken_reset(darken_t *ctx)
{
    DARKEN_FOREACH(ctx, { _DARKEN_MAYBE_DESTROY(_entity); });
    ctx->size = 0;
}

#endif // DARKEN_H
