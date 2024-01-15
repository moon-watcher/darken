#include "entity.h"

__attribute__((always_inline)) inline void de_state_enter(de_entity *const entity)
{
    de_state_exec(entity, enter);
}

__attribute__((always_inline)) inline void de_state_update(de_entity *const entity)
{
    de_state_exec(entity, update);
}

__attribute__((always_inline)) inline void de_state_leave(de_entity *const entity)
{
    de_state_exec(entity, leave);
    entity->state = 0;

#if DARKEN_ENTITY_TEMPDATA
#include "../config/free.h"
    free(entity->tempdata);
    entity->tempdata = 0;
#endif
}

__attribute__((always_inline)) inline void de_state_empty(de_entity *const this) {}