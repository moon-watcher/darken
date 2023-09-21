#include "state.h"
#include "entity.h"

#include "../config/free.h"

void *de_state_data(de_entity *const e, unsigned int size)
{
#if DARKEN_ENTITY_STATEDATA
    if (e->statedata == 0)
        e->statedata = malloc(size);

    return e->statedata;
#else
    return 0;
#endif
}

void de_state_free(de_entity *const e)
{
#if DARKEN_ENTITY_STATEDATA
    if (e->statedata != 0)
        return;

    free(e->statedata);
    e->statedata = 0;
#else
    ;
#endif
}

void de_state_change(de_entity *const e, const de_state *const s)
{
    de_state *const state = e->state;

    if (state == s)
        return;

    de_state_f const leave_s = state->leave;

    if (leave_s != 0)
        leave_s(e);

    de_state_set(e, s);
}

void de_state_set(de_entity *const e, const de_state *const s)
{
    de_state_free(e);

    e->state = (de_state *)s;
    e->update = s->update ?: ({ void f() {}; f; });

    de_state_f const enter_s = s->enter;

    if (enter_s != 0)
        enter_s(e);
}

void de_state_update(de_entity *const e)
{
    e->update(e);
}

void de_state_destruct(de_entity *const e)
{
    de_state_f const leave_s = e->state->leave;
    de_state_f const leave_x = e->xtor->leave;

    if (leave_s != 0)
        leave_s(e);

    de_state_free(e);

    if (leave_s != leave_x && leave_x != 0)
        leave_x(e);
}