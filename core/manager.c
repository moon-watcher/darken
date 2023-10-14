#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/darken.h"

#include "../libs/culist.h"

void de_manager_init(de_manager *const this, unsigned int maxEntities, unsigned int objectSize)
{
    culist_init(&this->cul, maxEntities, objectSize + sizeof(de_entity));
}

void de_manager_end(de_manager *const this)
{
    culist_end(&this->cul, de_state_leave);
}

void de_manager_reset(de_manager *const this)
{
    culist_reset(&this->cul, de_state_leave);
}

void de_manager_update(de_manager *const this)
{
    culist_iterator(&this->cul, de_state_update);
}

void de_manager_iterate(de_manager *const this, void (*iterator)())
{
    culist_iterator(&this->cul, iterator);
}

de_entity *de_manager_entity_create(de_manager *const this, const de_state *const state)
{
    de_entity *const entity = culist_add(&this->cul, 0);

// #if DARKEN_ENTITY_DATA
    memset(entity->data, 0, this->cul.objectSize - sizeof(de_entity));
// #endif

    entity->manager = this;
    entity->state = (de_state *)state;

    de_state_enter(entity);

    return entity;
}

void *de_manager_entity_delete(de_manager *const this, de_entity *const entity)
{
    return culist_remove(&this->cul, entity, de_state_leave);
}