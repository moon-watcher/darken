#include "manager.h"
#include "../debug.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(&this->list, sizeof(de_entity) + bytes);
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
    uclist_end(&this->list);
}

//

de_entity *de_manager_newEntity(de_manager *const this)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_ERROR("manager uclist_alloc() is null");
    }
    else
    {
        de_entity_setManager(entity, this);
        de_entity_setState(entity, 0);
        de_entity_setDestructor(entity, 0);
    }

    return entity;
}

int de_manager_deleteEntity(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, de_entity_destroy);

#if DARKEN_WARNING
    switch (ret)
    {
    case -1:
        DARKEN_WARNING("Delete entity: entity not found");
        break;
    case -2:
        DARKEN_WARNING("Delete entity: manager, this->count is 0");
        break;
    }
#endif

    return ret;
}
