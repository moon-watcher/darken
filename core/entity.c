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

void de_entity_destructor(de_entity *const this, de_state state)
{
    this->destructor = state;
}

void de_entity_set(de_entity *const this, de_state state)
{
    if (state == 0)
    {
        state = ({void *f() { return f; } f; });
    }

    this->handler = state;
}

void *de_entity_exec(de_entity *const this, de_state state)
{
    if (state == 0)
    {
        return 0;
    }

    return state(this, this->data);
}
