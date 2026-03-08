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
        entity->index = i;

        if (DE_STATE_IS_ACTIVE(state))
        {
            de_state aux = state(entity->data);

            if (DE_STATE_NEED_UPDATE(aux))
                entity->state = aux;
        }
        else if (DE_STATE_IS_PAUSED(state))
        {
            uint16_t aux = $->pause_index++;

            items[i] = items[aux];
            items[i]->index = i;
            items[aux] = entity;
            entity->index = aux;
        }
        else if (DE_STATE_IS_DELETED(state))
        {
            uint16_t aux = --$->list.size;

            items[i] = items[aux];

            if (i < aux)
                items[i]->index = i;

            if (entity->destructor)
                entity->destructor(entity->data);
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
