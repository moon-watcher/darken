#include "entity.h"
#include "entity.status.h"
#include "../config.h"

void de_entity_set(de_entity *const this, de_state *state)
{
    this->state = state;
    this->status = _DARKEN_ENTITY_STATUS_SET;
}

void de_entity_delete(de_entity *const this)
{
    this->status = _DARKEN_ENTITY_STATUS_DELETE;
}

void de_entity_skipUpdate(de_entity *const this)
{
    this->status = _DARKEN_ENTITY_STATUS_SKIP;
}