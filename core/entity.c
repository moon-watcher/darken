#include "entity.h"
#include "../debug.h"

void *de_entity_null()
{
    DARKEN_WARNING("Entity: exec de_entity_null()");

    return de_entity_null;
}

inline __attribute__((always_inline)) void *de_entity_update(de_entity *const entity)
{
    return entity->state = entity->state(entity, entity->data);
}

void *de_entity_setState(de_entity *const this, void *(*state)())
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: set null state");
    }

    return this->state = state ?: de_entity_null;
}

void *de_entity_setDestructor(de_entity *const this, void *(*destructor)())
{
    return this->destructor = destructor ?: de_entity_null;
}

void *de_entity_exec(de_entity *const this, void *(*exec)())
{
    if (exec == 0)
    {
        DARKEN_ERROR("Entity: exec null state");
    }

    return this->state = exec ? exec(this, this->data) : de_entity_null;
}

void *de_entity_destroy(de_entity *const entity)
{
    entity->state = entity->destructor ? entity->destructor(entity, entity->data) : de_entity_null;
    entity->destructor = de_entity_null;

    return entity->state;
}
