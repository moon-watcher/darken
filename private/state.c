#include "state.h"

__attribute__((always_inline)) inline void dep_state_enter(de_entity *const entity)
{
    if (entity->state->enter != 0)
        entity->state->enter(entity);
}

__attribute__((always_inline)) inline void dep_state_leave(de_entity *const entity)
{
    if (entity->state->leave != 0)
        entity->state->leave(entity);

    entity->state = 0;

#include "../config/darken.h"
#if DARKEN_ENTITY_TEMPDATA
#include "../config/free.h"
    free(entity->tempdata);
    entity->tempdata = 0;
#endif
}