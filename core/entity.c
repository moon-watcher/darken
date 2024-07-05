#include "entity.h"

void de_entity_set(de_entity *const this, de_state *const state)
{
    this->state = state;
}

void de_entity_delay(de_entity *const this)
{
    this->ctrl = 1;
}

void de_entity_delete(de_entity *const this)
{
    this->ctrl = 2;
}