#include "manager.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(&this->list, sizeof(de_entity) + bytes);
    this->pauseSize = 0;
}

de_entity *de_manager_new(de_manager *const this, de_state state, de_state destructor)
{
    return de_entity_destructor(de_entity_set(uclist_alloc(&this->list), state), destructor);
}

void de_manager_update(de_manager *const this)
{
    de_entity **const items = de_manager_getItems(this);
    unsigned i = this->pauseSize, size = de_manager_getSize(this);

    // if (this->debug)
    //     kprintf("%d %d ", i, size);

    while (i < size)
    {
        de_entity *const entity = items[i++];
        de_state const state = entity->state;

        if (!state)
        {
            --size;
            uclist_removeByIndex(&this->list, --i);
            de_entity_set(entity, entity->destructor ?: de_state_empty);
        }

        de_entity_update(entity);

        if (entity->state == de_state_empty)
        {
            void *const swap = items[i - 1];
            items[i - 1] = items[this->pauseSize];
            items[this->pauseSize] = swap;

            ++this->pauseSize;
        }
    }
}

void *de_manager_resumeEntity(de_manager *const this, de_entity *const pausedEntity)
{
    de_entity **const items = de_manager_getItems(this);
    unsigned i = 0, pauseSize = this->pauseSize, size = de_manager_getSize(this);

    while (i < pauseSize)
    {
        de_entity *const entity = items[i];

        if (pausedEntity == entity)
        {            
            --this->pauseSize;
            items[i] = items[this->pauseSize];
            return items[this->pauseSize] = entity;
            // void *const swap = items[--this->pauseSize];
            // items[i - 1] = items[--this->pauseSize] = entity;
            // items[this->pauseSize] = swap;

        }
        ++i;
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
