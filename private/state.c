#include "state.h"

#include "../services/free.h"
#include "../services/malloc.h"

__attribute__((always_inline)) inline void dep_state_enter(de_entity *const entity)
{
    if (entity->state->dm_size > 0)
        entity->statedata = malloc(entity->state->dm_size);

    if (entity->state->enter != 0)
        entity->state->enter(entity, entity->data, entity->statedata);
}

__attribute__((always_inline)) inline void dep_state_leave(de_entity *const entity)
{
    if (entity->state->leave != 0)
        entity->state->leave(entity, entity->data, entity->statedata);

    if (entity->state->dm_size > 0)
        free(entity->statedata);

    entity->state = &de_state_empty;
    entity->statedata = 0;
}