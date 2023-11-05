#include "entity.h"

#include "../config/free.h"

__attribute__((always_inline)) inline de_entity *de_state_update(de_entity *const entity)
{
    if (entity->state->update != 0)
        entity->state->update(entity);

    return entity;
}

__attribute__((always_inline)) inline de_entity *de_state_leave(de_entity *const entity)
{
    if (entity->state->leave != 0)
        entity->state->leave(entity);

    entity->state = 0;

#if DARKEN_ENTITY_TEMPDATA
    if (entity->tempdata != 0)
    {
        free(entity->tempdata);
        entity->tempdata = 0;
    }
#endif

    return entity;
}