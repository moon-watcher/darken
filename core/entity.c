#include "entity.h"
#include "manager.h"
#include "../debug.h"

inline __attribute__((always_inline)) void de_entity_update(de_entity *const entity)
{
    entity->handler = entity->handler(entity, entity->data);
}

int de_entity_delete(de_entity *const this)
{
    return de_manager_deleteEntity(this->manager, this);
}

void de_entity_destroy(de_entity *const entity)
{
    entity->destructor != 0 && entity->destructor(entity, entity->data);
}

void de_entity_setDestructor(de_entity *const this, de_state destructor)
{
    this->destructor = destructor;
}

void de_entity_setState(de_entity *const this, de_state state)
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: state not set");
    }
    else
    {
        this->handler = state ?: ({void *f() { return f; } f; });
    }
}

void de_entity_execState(de_entity *const this, de_state exec)
{
    if (exec == 0)
    {
        DARKEN_WARNING("Entity: exec not set");
    }
    else
    {
        exec(this, this->data);
    }
}
