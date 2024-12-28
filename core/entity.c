#include "entity.h"
#include "../config.h"

de_entity *de_entity_state(de_entity *const this, de_state_f state)
{
    if (this != 0)
    {
        this->state = state;
        return this;
    }

    DARKEN_LOG("de_entity_state(): invalid entity");
    return 0;
}

de_entity *de_entity_destructor(de_entity *const this, de_state_f state)
{
    if (this != 0)
    {
        this->destructor = state;
        return this;
    }

    DARKEN_LOG("de_entity_destructor(): invalid entity");
    return 0;
}

de_entity *de_entity_delete(de_entity *const this)
{
    return de_entity_state(this, 0);
}
