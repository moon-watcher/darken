#include "state.h"
#include "entity.h"

#include "../config/darken.h"
#include "../config/free.h"

void de_state_change(de_entity *const entity, const de_state *const state)
{
    de_state *const e_state = entity->state;

    if (e_state == state)
        return;

    de_state_f const leave_s = e_state->leave;

    if (leave_s != 0)
        leave_s(entity);

    de_state_set(entity, state);
}

void de_state_set(de_entity *const entity, const de_state *const state)
{
    de_state_free(entity);

    entity->state = (de_state *)state;
    entity->update = state->update ?: ({ void f() {}; f; });

    de_state_f const enter_s = state->enter;

    if (enter_s != 0)
        enter_s(entity);
}

void de_state_update(de_entity *const entity)
{
    entity->update(entity);
}

void de_state_destruct(de_entity *const entity)
{
    de_state_f const leave_s = entity->state->leave;
    de_state_f const leave_x = entity->xtor->leave;

    if (leave_s != 0)
        leave_s(entity);

    de_state_free(entity);

    if (leave_s != leave_x && leave_x != 0)
        leave_x(entity);
}

void *de_state_data(de_entity *const entity, unsigned int size)
{
#if DARKEN_STATE_STATEDATA
    if (entity->statedata == 0)
        entity->statedata = malloc(size);

    return entity->statedata;
#endif
    return 0;
}

void de_state_free(de_entity *const entity)
{
#if DARKEN_STATE_STATEDATA
    if (entity->statedata != 0)
        return;

    free(entity->statedata);
    entity->statedata = 0;
#endif
}
