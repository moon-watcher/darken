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
    entity->index = $->list.size - 1;

    return entity;
}

void de_manager_update(de_manager *$)
{
    DE_MANAGER_ITERATE($, {
        ENTITY->index = INDEX;
        de_state state = ENTITY->state;

        if (DE_STATE_IS_ACTIVE(state))
            DE_STATE_NEED_UPDATE(state = state(ENTITY->data)) && (ENTITY->state = state);

        else if (DE_STATE_IS_PAUSED(state))
            de_entity_pause(ENTITY);

        else if (DE_STATE_IS_DELETED(state))
            de_entity_delete(ENTITY);
    });
}

void de_manager_pause(de_manager *$)
{
    $->pause_index = $->list.size;
}

void de_manager_resume(de_manager *$)
{
    $->pause_index = 0;
}

void de_manager_reset(de_manager *$)
{
    $->pause_index = 0;
    DE_MANAGER_ITERATEALL($, de_entity_delete(ENTITY));
}

void de_manager_end(de_manager *$)
{
    uclist_end(&$->list);
}
