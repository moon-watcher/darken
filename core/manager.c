#include "manager.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(&this->list, sizeof(de_entity) + bytes);
    this->pause = 0;
}

de_entity *de_manager_new(de_manager *const this, de_state state, de_state destructor)
{
    de_entity *const entity = uclist_alloc(&this->list);
    entity->manager = this;

    return de_entity_destructor(de_entity_set(entity, state), destructor);
}

void de_manager_update(de_manager *const this)
{
    if (this->pause)
        return;

    uclist *const list = &this->list;
    de_entity **const items = (de_entity **)list->items;
    unsigned *const size = &list->size;
    unsigned i = 0;

    while (i < *size)
    {
        de_entity *const entity = items[i++];

        if (!entity->state)
        {
            uclist_removeByIndex(list, --i);
            de_entity_set(entity, entity->destructor ?: de_state_empty);
        }

        de_entity_update(entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(&this->list, (void *)de_entity_delete);
    de_manager_update(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uclist_end(&this->list);
}

//

inline unsigned de_manager_getSize(de_manager *const this)
{
    return this->list.size;
}

inline unsigned de_manager_getCapacity(de_manager *const this)
{
    return this->list.capacity;
}

inline de_entity **de_manager_getItems(de_manager *const this)
{
    return (de_entity **)this->list.items;
}
