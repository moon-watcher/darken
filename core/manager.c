#include "entity.h"
#include "manager.h"
#include "../libs/culist.h"

__attribute__((always_inline)) inline static void _entity_update(de_entity *const this)
{
    this->update(this);
}

__attribute__((always_inline)) inline static void _entity_destruct(de_entity *const this)
{
#include "../private/xtor.h"
#include "../private/state.h"
    dep_state_leave(this);
    dep_xtor_leave(this);
}

void de_manager_init(de_manager *const this, unsigned objectSize)
{
#if DARKEN_ENTITY_DATA == 0
    objectSize = 0;
#endif
    culist_init(&this->cul, objectSize + sizeof(de_entity), 1);
}

void de_manager_end(de_manager *const this)
{
    culist_end(&this->cul, _entity_destruct);
}

void de_manager_reset(de_manager *const this)
{
    culist_reset(&this->cul, _entity_destruct);
}

void de_manager_update(de_manager *const this)
{
    culist_iterator(&this->cul, _entity_update);
}

void de_manager_iterate(de_manager *const this, void (*iterator)())
{
    culist_iterator(&this->cul, iterator);
}

de_entity *de_manager_new(de_manager *const this)
{
    de_entity *entity = culist_add(&this->cul);

    entity->update = &de_state_func;
    entity->state = &de_state_empty;
    entity->xtor = &de_state_empty;

#if DARKEN_ENTITY_FIXEDDATA
    memset(entity->fixeddata, 0, DARKEN_ENTITY_FIXEDDATA);
#endif

#if DARKEN_ENTITY_TEMPDATA
    entity->tempdata = 0;
#endif

#if DARKEN_ENTITY_DATA
    memset(entity->data, 0, this->cul.objectSize - sizeof(de_entity));
#endif

    return entity;
}

unsigned de_manager_delete(de_manager *const this, de_entity *const entity)
{
    return culist_remove(&this->cul, entity, _entity_destruct);
}