#include "darken.h"

void darken_init(de_manager *const this, unsigned short bytes)
{
    uclist_init_alloc(this, sizeof(de_entity) + bytes);
}

de_entity *darken_new(de_manager *const this, de_state state)
{
    de_entity *const entity = uclist_alloc(this);

    entity->state = state;

    return entity;
}

void darken_update(de_manager *const this)
{
    de_entity **const items = (de_entity **)this->items;
    unsigned short i = 0, size = this->size;

    while (i < size)
    {
        de_entity *const entity = items[i++];
        de_state state = entity->state;

        if (state == (de_state)1)
            continue;

        if (state)
            entity->state = state(entity->data);
        else
        {
            --size;
            uclist_removeByIndex(this, --i);
            entity->destructor && entity->destructor(entity->data);
        }
    }
}

void darken_reset(de_manager *const this)
{
    uclist_iterator(this, ({ void d(de_entity *e) { e->state = 0; }; d; }));
    darken_update(this);
}

void darken_end(de_manager *const this)
{
    darken_reset(this);
    uclist_end(this);
}
