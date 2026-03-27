#include "manager.h"

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
    DE_MANAGER_ITERATE($, {
        ENTITY->index = INDEX;

        if (DE_STATE_IS_ACTIVE(STATE))
        {
            STATE = STATE(DATA);

            if (DE_STATE_NEED_UPDATE(STATE))
                ENTITY->state = STATE;
        }

        else if (DE_STATE_IS_PAUSED(STATE))
            de_entity_pause(ENTITY);

        else if (DE_STATE_IS_DELETED(STATE))
            de_entity_delete(ENTITY);
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

de_entity **de_manager_getEntities(de_manager *$)
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
