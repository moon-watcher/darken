#include "darken.h"


// State

void *de_state_empty()
{
    return de_state_empty;
}


// Manager

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init_alloc(&this->list, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state state, de_state destructor)
{
    de_entity *const entity = uclist_alloc(&this->list);

    entity->state = state;
    entity->destructor = destructor;

    return entity;
}

void de_manager_update(de_manager *const this)
{
    de_entity **const items = de_manager_getItems(this);
    unsigned i = 0, size = de_manager_getSize(this);

    while (i < size)
    {
        de_entity *const entity = items[i++];
        de_state state = entity->state;

        if (state == de_state_empty)
            continue;

        if (!state)
        {
            --size;
            uclist_removeByIndex(&this->list, --i);
            state = entity->destructor ?: de_state_empty;
        }

        entity->state = state(entity->data);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(&this->list, ({ void d(de_entity *e) { e->state = 0; }; d; }));
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
