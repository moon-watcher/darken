#include "entity.h"

void de_entity_set(de_entity *const this, void (*state[static 1])())
{
    this->state = state;
}

void de_entity_destructor(de_entity *const this, void (*destructor)())
{
    this->destructor = destructor;
}
