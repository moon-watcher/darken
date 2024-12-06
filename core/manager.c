#include "manager.h"
#include "../debug.h"

static void _destroy(de_entity *const this)
{
    this->destructor && this->destructor(this->data, this);
}

//

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
    uclist_iterator(&this->list, _destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    uclist_iterator(&this->list, _destroy, 1);
    uclist_end(&this->list);
}

//

de_entity *de_manager_new(de_manager *const this, de_state_f state)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_ERROR("Manager: uclist_alloc() is null");
        return 0;
    }

    entity->manager = this;
    return de_entity_set(entity, state);
}

int de_manager_delete(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, _destroy);

#if DARKEN_WARNING
    switch (ret)
    {
    case -1:
        DARKEN_WARNING("Manager: deleteEntity: entity not found");
        break;
    case -2:
        DARKEN_WARNING("Manager: deleteEntity: count is 0");
        break;
    }
#endif

    return ret;
}
