#include "../darken.h"

static deStateAPI *exec(deEntity_t *const a, deState_f const b)
{
    deState_exec(a, b);
    return (deStateAPI *)deState;
}

static deStateAPI *enter(deEntity_t *const a)
{
    deState_enter(a);
    return (deStateAPI *)deState;
}

static deStateAPI *update(deEntity_t *const a)
{
    deState_update(a);
    return (deStateAPI *)deState;
}

static deStateAPI *leave(deEntity_t *const a)
{
    deState_leave(a);
    return (deStateAPI *)deState;
}

const deStateAPI *const deState = &(deStateAPI){exec, enter, update, leave};