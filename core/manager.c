#include "debug.h"
#include "manager.h"
#include "entity.h"
#include "../priv/common.h"
#include "../config.h"


void de_manager_init(de_manager *const this, unsigned bytes, unsigned datasize)
{
    uclist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    _DARKEN_COMMON_INIT(this, datasize);
}

void de_manager_update(de_manager *const this)
{
    uclist_iterator(&this->list, de_entity_update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(&this->list, de_entity_destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    _DARKEN_COMMON_END(this, de_manager);
}

//

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_DEBUG_ERROR("manager uclist_alloc() is null");
    }
    else
    {
        de_entity_init(entity, this, desctructor);
    }

    return entity;
}

void de_manager_delete(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, 0);

    if (ret == -1)
    {
        DARKEN_DEBUG_WARNING("manager, entity not found");
    }
    else if (ret == -2)
    {
        DARKEN_DEBUG_WARNING("manager, this->count is 0");
    }
}
