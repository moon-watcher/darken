#include "manager.h"

#include "../libs/culist.h"

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

    this->entityBytes = entityBytes + sizeof(de_entity);
    culist_init(&this->cul);
}

de_entity *de_manager_new(de_manager *const this)
{
    return de_manager_newEx(this, this->entityBytes);
}

de_entity *de_manager_newEx(de_manager *const this, unsigned size)
{
    de_entity *entity = culist_add(&this->cul, size);

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
    memset(entity->data, 0, this->entityBytes - sizeof(de_entity));
#endif

    return entity;
}

void de_manager_update(de_manager *const this)
{
    upiterator_culist(&this->cul, _entity_update, 1);
}

unsigned de_manager_delete(de_manager *const this, de_entity *const entity)
{
    int index = culist_find(&this->cul, entity);

    if (index < 0)
        return 0;

    _entity_destruct(this->cul.items[index]);

    return culist_removeById(&this->cul, index);
}

void de_manager_reset(de_manager *const this)
{
    upiterator_culist(&this->cul, _entity_destruct, 1);
    culist_reset(&this->cul);
}

void de_manager_end(de_manager *const this)
{
    upiterator_culist(&this->cul, _entity_destruct, 1);
    culist_end(&this->cul);
}