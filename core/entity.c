#include "entity.h"

inline __attribute__((always_inline)) void *de_entity_update(de_entity *const entity)
{
    return entity->handler = entity->handler(entity->data, entity);
}

void *de_entity_delete(de_entity *const entity)
{
    return entity->destructor != 0 ? entity->destructor(entity->data, entity) : 0;
}
