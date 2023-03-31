#include "state.h"
#include "entity.h"

void deState_enter(deEntity_t *const e)
{
    deState_f func = e->state->enter;

    if (func) func(e);
}

void deState_update(deEntity_t *const e)
{
    deState_f func = e->state->update;

    if (func) func(e);
}

void deState_leave(deEntity_t *const e)
{
    deState_f func = e->state->leave;

    if (func) func(e);
}