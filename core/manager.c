#include "entity.h"
#include "manager.h"
#include "../libs/culist.h"
#include "../private/entity.h"

void de_manager_init(de_manager *const this, unsigned objectSize)
{
#if DARKEN_ENTITY_DATA == 0
    objectSize = 0;
#endif
    culist_init(&this->cul, objectSize + sizeof(de_entity), 1);
}

void de_manager_end(de_manager *const this)
{
    culist_end(&this->cul, dep_entity_destruct);
}

void de_manager_reset(de_manager *const this)
{
    culist_reset(&this->cul, dep_entity_destruct);
}

void de_manager_update(de_manager *const this)
{
    culist_iterator(&this->cul, dep_entity_update);
}

void de_manager_iterate(de_manager *const this, void (*iterator)())
{
    culist_iterator(&this->cul, iterator);
}

de_entity *de_manager_new(de_manager *const this)
{
    de_entity *entity = culist_add(&this->cul);

#if DARKEN_ENTITY_FIXEDDATA
    memset(entity->fixeddata, 0, DARKEN_ENTITY_FIXEDDATA);
#endif

#if DARKEN_ENTITY_TEMPDATA
    entity->tempdata = 0;
#endif

#if DARKEN_ENTITY_DATA
    memset(entity->data, 0, this->cul.objectSize - sizeof(de_entity));
#endif

    entity->state = (de_state *)&de_state_empty;
    entity->xtor = (de_state *)&de_state_empty;
    entity->update = entity->xtor->update ?: de_state_null;

    return entity;
}

unsigned de_manager_delete(de_manager *const this, de_entity *const entity)
{
    return culist_remove(&this->cul, entity, dep_entity_destruct);
}