#include "entity.h"
#include "manager.h"

inline __attribute__((always_inline)) void de_entity_update(de_entity *const entity)
{
    entity->handler = entity->handler(entity, entity->data);
}

int de_entity_delete(de_entity *const this)
{
    return de_manager_delete(this->manager, this);
}

void de_entity_destructor(de_entity *const this, void *(*destructor)())
{
    this->destructor = destructor;
}

void de_entity_set(de_entity *const this, void *(*state)())
{
    if (state == 0)
    {
        state = ({void *f() { return f; } f; });
    }

    this->handler = state;
}

void *de_entity_exec(de_entity *const this, void *(*exec)())
{
    if (exec == 0)
    {
        return 0;
    }

    return exec(this, this->data);
}
