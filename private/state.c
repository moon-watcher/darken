#include "state.h"

void dep_state_enter(de_entity *const entity)
{
    if (entity != 0 && entity->state != 0 && entity->state->enter != 0)
        entity->state->enter(entity);
}

void dep_state_leave(de_entity *const entity)
{
    if (entity != 0 && entity->state != 0 && entity->state->leave != 0)
        entity->state->leave(entity);

    entity->state = &de_state_empty;

#include "../config/entity.h"
#if DARKEN_ENTITY_TEMPDATA
#include "../services/free.h"
    free(entity->tempdata);
    entity->tempdata = 0;
#endif
}