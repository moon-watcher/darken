#include "state.h"

#include "../config/darken.h"
#include "../config/free.h"

__attribute__((always_inline)) inline void de_state_enter(de_entity *const entity)
{
    if (entity->state->enter != 0)
        entity->state->enter(entity);
}

__attribute__((always_inline)) inline void de_state_leave(de_entity *const entity)
{
    if (entity->state->leave != 0)
        entity->state->leave(entity);
        
    entity->state = 0;

#if DARKEN_ENTITY_TEMPDATA
    free(entity->tempdata);
    entity->tempdata = 0;
#endif
}