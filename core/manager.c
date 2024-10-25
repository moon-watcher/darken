#include "manager.h"
#include "../debug.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_initAlloc(&this->list, sizeof(de_entity) + bytes);
}

void de_manager_update(de_manager *const this)
{
    void _entity_update(de_entity *const this)
    {
        this->state(this, this->data);
    }

    uclist_iterator(&this->list, _entity_update, 1);
}

void de_manager_reset(de_manager *const this)
{
    void _entity_destroy(de_entity *const this)
    {
        if (this->destructor != 0)
            this->destructor(this, this->data);
    }

    uclist_iterator(&this->list, _entity_destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uclist_end(&this->list);
}

//

de_entity *de_manager_entityNew(de_manager *const this)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_ERROR("manager uclist_alloc() is null");
    }

    return entity;
}

int de_manager_entityDelete(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, 0);

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
