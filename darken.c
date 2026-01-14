#include "darken.h"

void darken_init(de_manager *$, uint16_t bytes)
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
    uint16_t i = $->size;

    while (i--)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

        if (DE_STATE_IS_ACTIVE(state))
        {
            de_state aux = state(entity->data);
            DE_STATE_NEED_UPDATE(aux) && (entity->state = aux);
        }
        else if (DE_STATE_IS_DELETED(state))
        {
            uclist_removeByIndex($, i);
            entity->destructor && entity->destructor(entity->data);
        }
    }
}

void darken_reset(de_manager *$)
{
    uclist_iterator($, ({ void d(de_entity *e) { e->state = DE_STATE_DELETE; }; d; }));
    darken_update($);
}

void darken_end(de_manager *$)
{
    darken_reset($);
    uclist_end($);
}
