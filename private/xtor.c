#include "xtor.h"

void dep_xtor_enter(de_entity *const entity)
{
    if (entity->xtor->enter != 0)
        entity->xtor->enter(entity, entity->data);
}

void dep_xtor_leave(de_entity *const entity)
{
    if (entity->xtor->leave != 0)
        entity->xtor->leave(entity, entity->data);

    entity->xtor = &de_state_empty;
}
