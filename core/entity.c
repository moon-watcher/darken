#include "entity.h"

de_state de_entity_exec(de_entity *const this)
{
    if (this->state == 0)
    {
        return 0;
    }

    return this->state(this->data, this);
}

void de_entity_delete(de_entity *const this)
{
    this->state = 0;
}

void de_entity_state(de_entity *const this, de_state_f state)
{
    this->state = state;
}

void de_entity_destructor(de_entity *const this, de_state_f destructor)
{
    this->destructor = destructor;
}
