#include "../darken.h"

static deManagerAPI *init(deManager_t *const a, unsigned b, unsigned c)
{
    deManager_init(a, b, c);
    return (deManagerAPI *)deManager;
}

static deManagerAPI *update(deManager_t *const a)
{
    deManager_update(a);
    return (deManagerAPI *)deManager;
}

static deManagerAPI *reset(deManager_t *const a)
{
    deManager_reset(a);
    return (deManagerAPI *)deManager;
}

static deManagerAPI *end(deManager_t *const a)
{
    deManager_end(a);
    return (deManagerAPI *)deManager;
}

const deManagerAPI *const deManager = &(deManagerAPI){init, update, reset, end};