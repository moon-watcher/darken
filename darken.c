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

static count;
void darken_update(de_manager *$)
{
    // uint16_t pause = $->pause_index;
    de_entity *const *restrict *items = (de_entity *const *)$->manager.items;
    uint16_t size = $->manager.size;

    // while (i-- >)
    for (uint16_t i = $->pause_index; i < size; i++)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

        if (DARKEN_IS_ACTIVE(state))
        {
            de_state aux = state(entity->data);
            DARKEN_NEED_UPDATE(aux) && (entity->state = aux);
        }
        else if (DARKEN_IS_PAUSED(state))
        {
            de_entity *temp = items[i];
            items[i] = items[$->pause_index];
            items[$->pause_index] = temp;

            ++$->pause_index;
        }
        else if (DARKEN_IS_DELETED(state))
        {
            uclist_removeByIndex($, i);
            entity->destructor && entity->destructor(entity->data);
        }
    }
}

void darken_reset(de_manager *$)
{
    uclist_iterator(&$->manager, ({ void d(de_entity *e) { e->state = DARKEN_DELETE; }; d; }));
    darken_update(&$->manager);
}

void darken_end(de_manager *$)
{
    darken_reset(&$->manager);
    uclist_end(&$->manager);
}
