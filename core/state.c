#include "entity.h"

const de_state de_state_empty = {de_state_func, de_state_func, de_state_func};

__attribute__((always_inline)) inline void de_state_update(de_entity *const entity)
{
    if (entity != 0 && entity->state != 0 && entity->state->update != 0)
        entity->state->update(entity);
}

__attribute__((always_inline)) inline void de_state_func(de_entity *const entity)
{
    // :)
}