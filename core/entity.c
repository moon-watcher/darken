#include "entity.h"
#include "../private/entity.h"
#include "../private/state.h"
#include "../config.h"

void de_entity_set(de_entity *const this, de_state *const state)
{
    this->state = state ?: &_de_state_empty;
    this->status = _DE_ENTITY_STATUS_SET;

    DARKEN_INFO("entity set");
}

void de_entity_delete(de_entity *const this)
{
    this->status = _DE_ENTITY_STATUS_DELETE;

    DARKEN_INFO("entity delete");
}

void de_entity_skipUpdate(de_entity *const this)
{
    this->status = _DE_ENTITY_STATUS_SKIP;

    DARKEN_INFO("entity skip");
}