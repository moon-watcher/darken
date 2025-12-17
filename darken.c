#include "darken.h"

void darken_init(de_manager *$, unsigned short bytes)
{
    uclist_init($, sizeof(de_entity) + bytes);
}

de_entity *darken_new(de_manager *$, de_state state)
{
    de_entity *entity = uclist_alloc($);

    return entity->state = state, entity;
}

void darken_update(de_manager *$)
{
    de_entity **items = $->items;
    unsigned short i = $->size;

    while (i--)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

        // state == (de_state)1: Pause

        if (state > (de_state)1)
            entity->state = state(entity->data);

        else if (!state)
        {
            uclist_removeByIndex($, i);
            entity->destructor && entity->destructor(entity->data);
        }
    }
}

void darken_reset(de_manager *$)
{
    uclist_iterator($, ({ void d(de_entity *e) { e->state = 0; }; d; }));
    darken_update($);
}

void darken_end(de_manager *$)
{
    darken_reset($);
    uclist_end($);
}
