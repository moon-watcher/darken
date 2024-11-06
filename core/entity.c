#include "entity.h"
#include "manager.h"
#include "../debug.h"

static void _nullf() {}

//

inline __attribute__((always_inline)) void de_entity_update(de_entity *const entity)
{
    entity->handler(entity, entity->data);
}

int de_entity_delete(de_entity *const this)
{
    return de_manager_deleteEntity(this->manager, this);
}

void de_entity_destroy(de_entity *const entity)
{
    if (entity->destructor == 0)
    {
        DARKEN_WARNING("Entity: destructor is 0. Who cares?");
        return;
    }

    entity->destructor(entity, entity->data);
}

void de_entity_setState(de_entity *const this, de_state state)
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: state is null. Not set");
    }

    this->handler = state ?: _nullf;
}

void de_entity_setManager(de_entity *const this, de_manager *const manager)
{
    this->manager = manager;
}

void de_entity_setDestructor(de_entity *const this, de_state state)
{
    this->destructor = state ?: _nullf;
}

void de_entity_execState(de_entity *const this, de_state state)
{
    if (state == 0)
    {
        DARKEN_WARNING("Entity: exec not set");
        return;
    }

    state(this, this->data);
}
