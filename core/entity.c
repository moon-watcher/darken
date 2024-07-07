#include "entity.h"
#include "../NOAPI/state.h"

de_entity *de_entity_set(de_entity *const this, de_state *const state)
{
    this->state = state ?: &de_NOAPI_state_empty;
    this->ctrl = 3;

    return this;
}

de_entity *de_entity_delay(de_entity *const this)
{
    this->ctrl = 1;

    return this;
}

de_entity *de_entity_delete(de_entity *const this)
{
    this->ctrl = 2;

    return this;
}