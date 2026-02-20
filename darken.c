#include "darken.h"

void darken_init(de_manager *$, uint16_t bytes)
{
    uclist_init(&$->manager, sizeof(de_entity) + bytes);
    // $->pause_index = 0;
}

de_entity *darken_new(de_manager *$)
{
    return uclist_alloc(&$->manager);
}

void darken_update(de_manager *$)
{
    de_entity *const *restrict items = (de_entity *const *)$->manager.items;
    uint16_t i = $->manager.size;
    // uint16_t pause = $->pause_index;

    while (i--)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

        if (DE_STATE_IS_ACTIVE(state))
        {
            de_state aux = state(entity->data);
            DE_STATE_NEED_UPDATE(aux) && (entity->state = aux);
        }
        // else if (DE_STATE_IS_PAUSED(state))
        // {
        //     move to first position and increase $->pause_index
        // }
        else if (DE_STATE_IS_DELETED(state))
        {
            uclist_removeByIndex($, i);
            entity->destructor && entity->destructor(entity->data);
        }
    }
}

void darken_reset(de_manager *$)
{
    uclist_iterator(&$->manager, ({ void d(de_entity *e) { e->state = DE_STATE_DELETE; }; d; }));
    darken_update(&$->manager);
}

void darken_end(de_manager *$)
{
    darken_reset(&$->manager);
    uclist_end(&$->manager);
}
