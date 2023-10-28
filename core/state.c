#include "entity.h"

__attribute__((always_inline)) inline de_entity *de_state_update(de_entity *const entity)
{
    if (entity->state->update != 0)
        entity->state->update(entity);

    return entity;
}

__attribute__((always_inline)) inline de_entity *de_xtor_enter(de_entity *const entity)
{
    if (entity->xtor->enter != 0)
        entity->xtor->enter(entity);

    return entity;
}

__attribute__((always_inline)) inline de_entity *de_xtor_update(de_entity *const entity)
{
    if (entity->xtor->update != 0)
        entity->xtor->update(entity);

    return entity;
}