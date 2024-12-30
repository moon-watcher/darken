#include "entity.h"
#include "../assert.h"

de_entity *de_entity_set(de_entity *const this, de_state_f state)
{
    _DARKEN_ASSERT(this, 0, "Invalid entity");
    this->state = state;
    return this;
}

de_entity *de_entity_destructor(de_entity *const this, de_state_f state)
{
    _DARKEN_ASSERT(this, 0, "Invalid entity");
    this->destructor = state;
    return this;
}

de_entity *de_entity_delete(de_entity *const this)
{
    return de_entity_set(this, 0);
}
