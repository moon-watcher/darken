#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

#include "../libs/culist.h"

void de_manager_init(de_manager *const this, unsigned int maxEntities, unsigned int objectSize)
{
    this->pause = 0;
    culist_init(&this->cul, maxEntities, objectSize + sizeof(de_entity));
    de_manager_free(this);
}

void de_manager_end(de_manager *const this)
{
    culist_end(&this->cul, de_state_destruct);
    de_manager_free(this);
}

void de_manager_reset(de_manager *const this)
{
    culist_reset(&this->cul, de_state_destruct);
}

void de_manager_update(de_manager *const this)
{
    if (this->pause == 0)
        culist_iterator(&this->cul, de_state_update);
}

void *de_manager_data(de_manager *const this, unsigned int size)
{
#if DARKEN_MANAGER_DATA
    if (this->data == 0)
        this->data = malloc(size);

    return this->data;
#endif
    return 0;
}

void de_manager_free(de_manager *const this)
{
#if DARKEN_MANAGER_DATA
    if (this->data != 0)
        return;

    free(this->data);
    this->data = 0;
#endif
}

de_entity *de_manager_entity_create(de_manager *const this, const de_state *const state)
{
    de_entity *entity;

    if (this == 0)
        entity = malloc(sizeof(de_entity));
    else
    {
        entity = culist_add(&this->cul, 0);
        memset(entity->data, 0, this->cul.objectSize - sizeof(de_entity));
    }

    entity->xtor = (de_state *)state;
    entity->manager = this;

    de_state_set(entity, state);

    return entity;
}

void de_manager_entity_delete(de_manager *const this, de_entity *const entity)
{
    if (this != 0)
        culist_remove(&this->cul, entity, de_state_destruct);
    else
    {
        de_state_destruct(entity);
        free(entity);
    }
}

void de_manager_entity_iterator(de_manager *const this, void (*iterator)(de_entity *const entity))
{
    culist_iterator(&this->cul, iterator);
}