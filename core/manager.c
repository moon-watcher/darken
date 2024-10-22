#include "../debug.h"
#include "../config.h"
#include "../core/manager.h"
#include "../core/entity.h"
#include "../priv/declarations.h"
#include "../priv/macros.h"

void de_manager_init(de_manager *const this, unsigned bytes, unsigned datasize)
{
    uclist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    _COMMON_INIT(this, datasize);
}

de_entity *de_manager_new(de_manager *const this, de_state desctructor)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity != 0)
    {
        _entity_init(entity, this, desctructor);
    }
    else
    {
        DARKEN_ERROR("manager uclist_alloc() is null");
    }

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uclist_iterator(&this->list, _entity_update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(&this->list, _entity_destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    _COMMON_END(this, de_manager);
}
