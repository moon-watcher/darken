#include "manager.h"
#include "debug.h"

void de_manager_init(de_manager *$, uint16_t bytes, uint16_t items)
{
    uclist_init_fixedAlloc(&$->list, sizeof(de_entity) + bytes, items);
    $->pause_index = 0;
}

de_entity *de_manager_new(de_manager *$)
{
    de_entity *entity = uclist_alloc(&$->list);

    if (entity)
    {
        entity->manager = $;
        entity->index = $->list.size - 1;
    }
    
    return entity;
}

void de_manager_update(de_manager *$)
{
    de_manager_iterate($, {
        ENTITY->index = INDEX;
        de_state state = ENTITY->state;

        if (de_state_is_active(state))
            de_entity_update(ENTITY, state);

        else if (de_state_is_paused(state))
            de_entity_pause(ENTITY);

        else if (de_state_is_deleted(state))
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
    de_manager_iterateAll($, de_entity_delete(ENTITY));
}

void de_manager_end(de_manager *$)
{
    uclist_end(&$->list);
}
