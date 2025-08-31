#include "manager.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(&this->list, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state state, de_state destructor)
{
    return de_entity_destructor(de_entity_set(uclist_alloc(&this->list), state), destructor);
}

void de_manager_update(de_manager *const this)
{
    de_entity **const items = de_manager_getItems(this);
    unsigned i = 0, size = de_manager_getSize(this);

    while (i < size)
    {
        de_entity *const entity = items[i++];
        de_state const state = entity->state;

        if (state == de_state_empty)
            continue;

        if (!state)
        {
            --size;
            uclist_removeByIndex(&this->list, --i);
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
