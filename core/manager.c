#include "manager.h"
#include "../config.h"

static void _destroy(de_entity *const entity)
{
    if (entity->destructor != 0)
    {
        entity->destructor(entity->data, entity);
    }
}

//

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(&this->list, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state_f state)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_ERROR("Manager: uclist_alloc() is null");
        return 0;
    }

    entity->manager = this;
    entity->state = state;

    return entity;
}

void de_manager_update(de_manager *const this)
{
    unsigned index = 0;

    while (index < this->list.count)
    {
        de_entity *const entity = this->list.items[index];

        if (entity->state != 0)
        {
            entity->state = entity->state(entity->data, entity);
            ++index;
        }
        else
        {
            uclist_removeByIndex(&this->list, index, _destroy);
        }
    }
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
