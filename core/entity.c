#include "entity.h"

de_entity *de_entity_set(de_entity *const this, void (*state[static 1])())
{
    this->state = state;

    return this;
}

de_entity *de_entity_destructor(de_entity *const this, void (*destructor)())
{
    this->destructor = destructor;

    return this;
}
