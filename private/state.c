#include "state.h"

#include "../services/free.h"

__attribute__((always_inline)) inline void dep_state_enter(de_entity *const entity)
{
    if (entity->state->enter != 0)
        entity->state->enter(entity, entity->data, entity->statedata);
}

__attribute__((always_inline)) inline void dep_state_leave(de_entity *const entity)
{
    if (entity->state->leave != 0)
        entity->state->leave(entity, entity->data, entity->statedata);

    entity->state = &de_state_empty;

    free(entity->statedata);
    entity->statedata = 0;
}