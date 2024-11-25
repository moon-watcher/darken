#include "entity.h"
#include "manager.h"

int de_entity_delete(de_entity *const this)
{
    return de_manager_deleteEntity(this->manager, this);
}

void de_entity_destructor(de_entity *const this, de_state destructor)
{
    this->destructor = destructor;
}

void de_entity_set(de_entity *const this, de_state state)
{
    this->handler = state ?: ({void *f() { return f; } f; });
}

void *de_entity_exec(de_entity *const this, de_state exec)
{
    return exec != 0 && exec(this, this->data);
}