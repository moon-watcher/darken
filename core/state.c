#include "state.h"
#include "entity.h"

void de_state_set(de_entity *const e, const de_state *const s)
{
    if (e->state->leave != 0)
        e->state->leave(e);

    de_state_force(e, s);
}

void de_state_force(de_entity *const e, const de_state *const s)
{
    void nullf() {}

    e->state = (de_state *)s;
    e->update = e->state->update ?: nullf;

    if (e->state->enter)
        e->state->enter(e);
}

void de_state_update(de_entity *const e)
{
    e->update(e);
}

void de_state_destruct(de_entity *const e)
{
    if (e->state->leave != 0)
        e->state->leave(e);

    if (e->state->leave != e->destructor && e->destructor != 0)
        e->destructor(e);
}