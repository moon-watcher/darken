#include "entity.h"
#include "../priv/declarations.h"

void de_entity_set(de_entity *const this, de_state state)
{
    this->state = state;
    this->timer = 0;
}

void de_entity_delete(de_entity *const this)
{
    _manager_delete(this->manager, this);
}
