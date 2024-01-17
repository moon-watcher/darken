#include "xtor.h"

__attribute__((always_inline)) inline void dep_xtor_enter(de_entity *const entity)
{
    if (entity->xtor->enter != 0)
        entity->xtor->enter(entity);
}

__attribute__((always_inline)) inline void dep_xtor_leave(de_entity *const entity)
{
    if (entity->xtor->leave != 0 /* && entity->xtor->leave != entity->state->leave*/)
        entity->xtor->leave(entity);

    entity->xtor = 0;
}
