#include "state.h"
#include "entity.h"

void deState_set(deEntity_t *const e, const deState_t *const s)
{
    deState_leave(e);
    deState_force(e, s);
}

void deState_force(deEntity_t *const e, const deState_t *const s)
{
    void nullf() {}

    e->state.enter  = s->enter  ?: nullf;
    e->state.update = s->update ?: nullf;
    e->state.leave  = s->leave  ?: nullf;

    deState_enter(e);
}

void deState_enter(deEntity_t *const e)
{
    e->state.enter(e);
}

void deState_update(deEntity_t *const e)
{
    e->state.update(e);
}

void deState_leave(deEntity_t *const e)
{
    e->state.leave(e);
}

void deState_destruct(deEntity_t *const e)
{
    deState_leave(e);

    if (e->state.leave != e->destructor)
        e->destructor(e);
}