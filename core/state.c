#include "state.h"
#include "entity.h"

void deState_set(deEntity_t *const e, const deState_t *const s)
{
    if (e->state->leave != 0)
        e->state->leave(e);

    deState_force(e, s);
}

void deState_force(deEntity_t *const e, const deState_t *const s)
{
    void nullf() {}

    e->state = (deState_t *) s;
    e->update = e->state->update ?: nullf;

    if (e->state->enter)
        e->state->enter(e);
}

void deState_update(deEntity_t *const e)
{
    e->update(e);
}

void deState_destruct(deEntity_t *const e)
{
    if (e->state->leave != 0)
        e->state->leave(e);

    if (e->state->leave != e->destructor && e->destructor != 0)
        e->destructor(e);
}