#include "entity.h"
#include "entity.status.h"
#include "../config.h"

void de_entity_set(de_entity *const this, de_state *state)
{
    this->state = state;
    this->status = _DARKEN_ENTITY_STATUS_SET;

    _DARKEN_DEBUG_INFO("entity set");
}

void de_entity_delete(de_entity *const this)
{
    this->status = _DARKEN_ENTITY_STATUS_DELETE;

    _DARKEN_DEBUG_INFO("entity delete");
}

void de_entity_skipUpdate(de_entity *const this)
{
    this->status = _DARKEN_ENTITY_STATUS_SKIP;

    _DARKEN_DEBUG_INFO("entity skip");
}