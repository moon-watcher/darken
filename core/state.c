#include "state.h"
#include "entity.h"


void deState_enter(deEntity_t *const e)
{
    deState_exec(e, e->state->enter);
}

void deState_update(deEntity_t *const e)
{
    deState_exec(e, e->state->update);
}

void deState_leave(deEntity_t *const e)
{
    deState_exec(e, e->state->leave);
}

void deState_exec(deEntity_t *const e, deState_f func)
{
    if (func == 0)
        return;

    func(e);
}

