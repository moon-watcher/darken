#include "darken.h"

void darken_init(de_manager *$, uint16_t bytes)
{
    uclist_init(&$->manager, sizeof(de_entity) + bytes);
    $->pause_index = 0;
}

de_entity *darken_new(de_manager *$)
{
    de_entity *entity = uclist_alloc(&$->manager);
    entity->manager = $;

    return entity;
}

void darken_update(de_manager *$)
{
    de_entity **items = $->manager.items;
    uint16_t size = $->manager.size;
    uint16_t actives = 0, paused = 0, deleted = 0;

    for (uint16_t i = $->pause_index; i < size; i++)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

        if (DARKEN_IS_ACTIVE(state))
        {
            de_state aux = state(entity->data);
            DARKEN_NEED_UPDATE(aux) && (entity->state = aux);
            actives++;
        }
        else if (DARKEN_IS_PAUSED(state))
        {
            darken_entity_pause(entity);
            paused++;
        }
        else if (DARKEN_IS_DELETED(state))
        {
            uclist_removeByIndex($, i);
            entity->destructor && entity->destructor(entity->data);
            deleted++;
        }
    }

#include "debug.h"
if (paused)
    PRINT("actives: %d  paused: %d  deleted: %d", actives, paused, deleted);
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

void darken_entity_resume(de_entity *entity)
{
    de_entity *temp = entity;
    de_manager *manager = entity->manager;
    de_entity **items = manager->manager.items;

    --manager->pause_index;
    entity = items[manager->pause_index];
    items[manager->pause_index] = temp;
}

void darken_entity_pause(de_entity *entity)
{
    de_entity *temp = entity;
    de_manager *manager = entity->manager;
    de_entity **items = manager->manager.items;

    entity = items[manager->pause_index];
    items[manager->pause_index] = temp;

    ++manager->pause_index;
}