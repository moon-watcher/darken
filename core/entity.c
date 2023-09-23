#include "state.h"
#include "entity.h"
#include "manager.h"

de_entity *de_entity_new(const de_state *const state)
{
    return de_manager_entity_create(0, state);
}

void de_entity_delete(de_entity *const this)
{
    de_manager_entity_delete(this->manager, this);
}