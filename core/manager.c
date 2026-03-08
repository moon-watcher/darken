#include "manager.h"

void de_manager_init(de_manager *$, uint16_t bytes)
{
    uclist_init(&$->list, sizeof(de_entity) + bytes);
    $->pause_index = 0;
}

de_entity *de_manager_new(de_manager *$)
{
    de_entity *entity = uclist_alloc(&$->list);
    entity->manager = $;
    entity->index = -1;

    return entity;
}

void de_manager_update(de_manager *$)
{
    de_entity **items = $->list.items;
    uint16_t i = $->list.size;

    while (i-- > $->pause_index)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

        // TODO correct implementation
        entity->index = i;

        if (DE_STATE_IS_ACTIVE(state))
        {
            de_state aux = state(entity->data);
            DE_STATE_NEED_UPDATE(aux) && (entity->state = aux);
        }
        else if (DE_STATE_IS_PAUSED(state))
        {
            items[i] = items[$->pause_index];
            items[$->pause_index++] = entity;
        }
        else if (DE_STATE_IS_DELETED(state))
        {
            // TODO: use ->index to remove entity from manager instead of searching it
            uclist_removeByIndex(&$->list, i);
            entity->destructor && entity->destructor(entity->data);
        }
    }
}

void de_manager_pause(de_manager *$)
{
    $->pause_index = $->list.size;
}

void de_manager_resume(de_manager *$)
{
    $->pause_index = 0;
}

void de_manager_iterate(de_manager *$, void (*iterator)())
{
    de_entity **items = $->list.items;
    uint16_t i = $->list.size;
    uint16_t first = $->pause_index;

    while (i-- > first)
        iterator(items[i]->data);
}

void de_manager_iterateAll(de_manager *$, void (*iterator)())
{
    de_entity **items = $->list.items;
    uint16_t i = $->list.size;
    uint16_t first = 0;

    while (i-- > first)
        iterator(items[i]->data);
}

void de_manager_reset(de_manager *$)
{
    uclist_iterator(&$->list, ({ void d(de_entity *e) { e->state = DE_STATE_DELETE; }; d; }));
    de_manager_update(&$->list);
}

void de_manager_end(de_manager *$)
{
    de_manager_reset(&$->list);
    uclist_end(&$->list);
}
