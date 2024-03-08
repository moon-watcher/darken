#include "manager.h"

#include "../libs/uplist.h"

#include "../private/xtor.h"
#include "../private/state.h"

__attribute__((always_inline)) inline static void _entity_update(de_entity *const this)
{
    this->update(this);
}

__attribute__((always_inline)) inline static void _entity_destruct(de_entity *const this)
{
    dep_state_leave(this);
    dep_xtor_leave(this);
}

//

void de_manager_init(de_manager *const this, unsigned entityBytes)
{
#if DARKEN_ENTITY_DATA == 0
    entityBytes = 0;
#endif

    uplist_init(&this->cul, entityBytes + sizeof(de_entity));
}

de_entity *de_manager_new(de_manager *const this)
{
    de_entity *entity = uplist_alloc(&this->cul);

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
    memset(entity->data, 0, this->cul.itemSize - sizeof(de_entity));
#endif

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(&this->cul, _entity_update, 1);
}

unsigned de_manager_delete(de_manager *const this, de_entity *const entity)
{
    int index = uplist_find(&this->cul, entity);

    if (index < 0)
        return 0;

    _entity_destruct(this->cul.items[index]);

    return uplist_remove(&this->cul, entity);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(&this->cul, _entity_destruct, 1);
    uplist_reset(&this->cul);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(&this->cul);
}