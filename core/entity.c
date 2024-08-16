#include "entity.h"
#include "../config.h"

void de_entity_set(de_entity *const this, de_state *const state)
{
    this->state = state ?: &de_state_empty;
    this->ctrl = 3;
    
    DARKEN_INFO("entity set");
}

void de_entity_delay(de_entity *const this)
{
    this->ctrl = 1;
}

void de_entity_delete(de_entity *const this)
{
    this->ctrl = 2;
    
    DARKEN_INFO("entity delete");
}