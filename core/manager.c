#include "manager.h"

#define LOOPITEMS(LIMIT, CODE)                 \
    de_entity **items = de_manager_getList($); \
    uint16_t i = de_manager_countAll($);       \
                                               \
    while (i-- > LIMIT)                        \
        CODE;

//

void de_manager_init(de_manager *$, uint16_t bytes)
{
    uclist_init(&$->list, sizeof(de_entity) + bytes);
    de_manager_resume($);
}

de_entity *de_manager_new(de_manager *$)
{
    de_entity *entity = uclist_alloc(&$->list);
    entity->manager = $;
    entity->index = $->list.size - 1;

    return entity;
}

void de_manager_update(de_manager *$)
{
    LOOPITEMS($->pause_index, {
        de_entity *entity = items[i];
        de_state state = entity->state;
        entity->index = i;

        if (DE_STATE_IS_ACTIVE(state))
        {
            state = state(entity->data);
            DE_STATE_NEED_UPDATE(state) && (entity->state = state);
        }

        else if (DE_STATE_IS_PAUSED(state))
            de_entity_pause(entity);

        else if (DE_STATE_IS_DELETED(state))
            de_entity_delete(entity);
    });
}

void de_manager_pause(de_manager *$)
{
    $->pause_index = de_manager_countAll($);
}

void de_manager_resume(de_manager *$)
{
    $->pause_index = 0;
}

void de_manager_iterate(de_manager *$, void (*iterator)())
{
    LOOPITEMS($->pause_index, iterator(items[i]->data));
}

void de_manager_iterateAll(de_manager *$, void (*iterator)())
{
    LOOPITEMS(0, iterator(items[i]->data));
}

de_entity **de_manager_getList(de_manager *$)
{
    return $->list.items;
}

uint16_t de_manager_getCapacity(de_manager *$)
{
    return $->list.capacity;
}

uint16_t de_manager_countAll(de_manager *$)
{
    return $->list.size;
}

uint16_t de_manager_count(de_manager *$)
{
    return $->list.size - $->pause_index;
}

void de_manager_reset(de_manager *$)
{
    de_manager_resume($);
    uclist_iterator(&$->list, ({ void d(de_entity *e) { e->state = DE_STATE_DELETE; }; d; }));
    de_manager_update($);
}

void de_manager_end(de_manager *$)
{
    de_manager_reset($);
    uclist_end(&$->list);
}
