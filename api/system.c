#include "../darken.h"

static deSystemAPI *init(deSystem_t *const a, deSystem_f const b, unsigned c, unsigned d)
{
    deSystem_init(a, b, c, d);
    return (deSystemAPI *)deSystem;
}

static deSystemAPI *update(deSystem_t *const a)
{
    deSystem_update(a);
    return (deSystemAPI *)deSystem;
}

static deSystemAPI *end(deSystem_t *const a)
{
    deSystem_end(a);
    return (deSystemAPI *)deSystem;
}

static deSystemAPI *delete(deSystem_t *const a, void *const b)
{
    deSystem_delete(a, b);
    return (deSystemAPI *)deSystem;
}

static deSystemAPI *add(deSystem_t *const a, void *const b[])
{
    deSystem_add(a, b);
    return (deSystemAPI *)deSystem;
}

// static deSystemAPI *add(deSystem_t *const a, ...)
// {
//     #define ADD(S, ...) deSystem_add(S, ## __VA_ARGS__)
//     ADD(a);
//     return (deSystemAPI *)deSystem;
// }

const deSystemAPI *const deSystem = &(deSystemAPI){init, update, end, delete, add};