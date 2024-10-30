#include "entity.h"
#include "../debug.h"

inline void *de_entity_null()
{
    DARKEN_WARNING("Entity: exec de_entity_null()");

    return de_entity_null;
}

inline void *de_entity_update(de_entity *const entity)
{
    return entity->state = entity->state(entity, entity->data);
}

inline void *de_entity_setState(de_entity *const this, void *(*state)())
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: set null state");
        state = de_entity_null;
    }

    return this->state = state;
}

inline void *de_entity_setDestructor(de_entity *const this, void *(*state)())
{
    return this->destructor = state ?: de_entity_null;
}

inline void *de_entity_exec(de_entity *const this, void *(*state)())
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: exec null state");
        state = de_entity_null;
    }

    return this->state = state(this, this->data);
}

inline void *de_entity_destroy(de_entity *const entity)
{
    entity->state = entity->destructor ? entity->destructor(entity, entity->data) : de_entity_null;
    entity->destructor = de_entity_null;

    return entity->state;
}
