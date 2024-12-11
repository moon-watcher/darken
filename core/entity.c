#include "entity.h"

inline __attribute__((always_inline)) de_state de_entity_update(de_entity *const this)
{
    return this->state = (this->state == 0 ?: this->state(this->data, this));
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
