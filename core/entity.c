#include "entity.h"
#include "manager.h"
#include "../debug.h"

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
    if (state == 0)
    {
        DARKEN_ERROR("Entity: state not set");
        state = ({void *f() { return f; } f; });
    }
    
    this->handler = state;
}

void de_entity_exec(de_entity *const this, de_state exec)
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