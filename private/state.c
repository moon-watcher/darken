#include "state.h"

#include "../config/entity.h"
#include "../services/free.h"

void dep_state_enter(de_entity *const entity)
{
    if (entity->state->enter != 0)
        entity->state->enter(entity, entity->data);
}

void dep_state_leave(de_entity *const entity)
{
    if (entity->state->leave != 0)
        entity->state->leave(entity, entity->data);

    entity->state = &de_state_empty;

    free(entity->tempdata);
    entity->tempdata = 0;
}