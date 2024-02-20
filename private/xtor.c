#include "xtor.h"

void dep_xtor_enter(de_entity *entity)
{
    if (entity != 0 && entity->xtor != 0 && entity->xtor->enter != 0)
        entity->xtor->enter(entity);
}

void dep_xtor_leave(de_entity *entity)
{
    if (entity != 0 && entity->xtor != 0 && entity->xtor->leave != 0)
        entity->xtor->leave(entity);

    entity->xtor = &de_state_empty;
}
