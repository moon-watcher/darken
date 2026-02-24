#include "darken.h"

void darken_init(de_manager *$, uint16_t bytes)
{
    uclist_init(&$->list, sizeof(de_entity) + bytes);
    $->pause_index = 0;
}

de_entity *darken_new(de_manager *$)
{
    de_entity *entity = uclist_alloc(&$->list);
    entity->manager = $;

    return entity;
}

void darken_update(de_manager *$)
{
    de_entity **items = $->list.items;
    uint16_t i = $->list.size;

    while (i-- > $->pause_index)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

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
            uclist_removeByIndex(&$->list, i);
            entity->destructor && entity->destructor(entity->data);
        }
    }
}

void darken_reset(de_manager *$)
{
    uclist_iterator(&$->list, ({ void d(de_entity *e) { e->state = DE_STATE_DELETE; }; d; }));
    darken_update(&$->list);
}

void darken_end(de_manager *$)
{
    darken_reset(&$->list);
    uclist_end(&$->list);
}






//

void darken_pause(de_manager *$)
{
    $->pause_index = $->list.size;
}

void darken_resume(de_manager *$)
{
    $->pause_index = 0;
}

void darken_iterate(de_manager *$, void (*iterator)())
{
    de_entity **items = $->list.items;
    uint16_t i = $->list.size;
    uint16_t first = $->pause_index;

    while (i-- > first)
        iterator(items[i]->data);
}

void darken_iterateAll(de_manager *$, void (*iterator)())
{
    uint16_t aux = $->pause_index;
    $->pause_index = 0;
    darken_iterate($, iterator);
    $->pause_index = aux;
}

void darken_entity_delete(de_entity *$)
{
    uclist_remove(&$->manager->list, $);
    $->destructor && $->destructor($->data);
}
