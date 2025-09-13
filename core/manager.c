#include "manager.h"
#include <genesis.h>

void de_manager_init(de_manager *const this, unsigned bytes)
{
    this->itemSize = sizeof(de_entity) + bytes;
    this->pack = 1;
    this->items = 0;
    this->size = 0;
    this->capacity = 0;
}

de_entity *de_manager_new(de_manager *const this, de_state state, de_state destructor)
{
    de_entity *entity = this->items[this->size];

    if (this->size < this->capacity)
        ++this->size;
    else if ((entity = malloc(this->itemSize)) != 0)
    {
        // resize //
        void **ptr = malloc((this->capacity + 1) * sizeof(void *));

        if (ptr == 0)
            return 0;

        memcpy(ptr, this->items, this->capacity * sizeof(void *));
        free(this->items);

        this->items = ptr;
        ++this->capacity;
        ////

        this->items[this->size++] = entity;
    }
    else
        return 0;

    memset(entity, 0, this->itemSize);
    
    entity->state = state;
    entity->destructor = destructor;

    return entity;
}

void de_manager_update(de_manager *const this)
{
    de_entity **const items = de_manager_getItems(this);
    unsigned i = 0;

    while (i < this->size)
    {
        de_entity *const entity = items[i++];
        de_state state = entity->state;

        if (state == de_state_empty)
            continue;

        if (!state)
        {
            --this->size;
            --i;

            void *const swap = this->items[i];
            this->items[i] = this->items[this->size];
            this->items[this->size] = swap;

            state = entity->destructor ?: de_state_empty;
        }

        entity->state = state(entity->data);
    }
}

void de_manager_reset(de_manager *const this)
{
    for (unsigned i = 0; i < this->size; ++i)
    {
        de_entity *const entity = this->items[i];
        entity->state = 0;
    }
    
    de_manager_update(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    while (this->itemSize && this->capacity--)
        free(this->items[this->capacity]);

    free(this->items);
}

//

inline unsigned de_manager_getSize(de_manager *const this)
{
    return this->size;
}

inline unsigned de_manager_getCapacity(de_manager *const this)
{
    return this->capacity;
}

inline de_entity **de_manager_getItems(de_manager *const this)
{
    return (de_entity **)this->items;
}
