#include "entity.h"

void de_entity_set(de_entity *const this, de_state state)
{
    this->state = state;
    this->timer = 0;
}

void de_entity_delete(de_entity *const this)
{
    _de_manager_delete(this->manager, this);
}
