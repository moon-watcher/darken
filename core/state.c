#include "state.h"
#include "entity.h"

void de_state_set(de_entity *const entity, const de_state *const newState)
{
    if (entity->state == newState)
        return;

    de_state_leave(entity);
    entity->state = (de_state *)newState;
    de_state_enter(entity);
}

void de_state_enter(de_entity *const entity)
{
    entity->state->enter(entity);
}

void de_state_update(de_entity *const entity)
{
    entity->state->update(entity);
}

void de_state_leave(de_entity *const entity)
{
    entity->state->leave(entity);
}


#include "../config/free.h"
#include "../config/malloc.h"
#include "../config/darken.h"

void *de_state_data(de_entity *const entity, unsigned int size)
{
#if DARKEN_STATE_STATEDATA
    return entity->statedata = malloc(size);
#endif
    return 0;
}

void de_state_free(de_entity *const entity)
{
#if DARKEN_STATE_STATEDATA
    free(entity->statedata);
#endif
}
