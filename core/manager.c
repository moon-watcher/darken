#include "manager.h"

// de_manager_def list1 = DEFIN(sizeof(struct lamp), 50);
// de_manager_def list2 = DEFIN(sizeof(struct player), 50);
// de_manager_def list3 = DEFIN(sizeof(struct enemy), 50);
// de_manager_def list4 = DEFIN(400, 50);

// de_manager_init(&g_manager_lamps,   &list1);
// de_manager_init(&g_manager_player,  &list2);
// de_manager_init(&g_manager_enemies, &list3);
// de_manager_init(&g_manager_stuff,   &list4);


void de_manager_init(de_manager *$, de_manager_def *def)
{
    $->list = def->list;
    $->itemSize = def->itemSize;
    $->capacity = def->capacity;
    $->size = 0;
    $->pause_index = 0;
}

de_entity *de_manager_new(de_manager *$)
{
    if ($->size >= $->capacity) return NULL;

    uint8_t *ptr = (uint8_t *)$->list + $->size * $->itemSize;
    memset(ptr, 0, $->itemSize);

    de_entity *e = (de_entity *)ptr;
    e->manager = $;
    e->index = $->size++;
    return e;
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

void de_manager_pause(de_manager *$)   { $->pause_index = $->size; }
void de_manager_resume(de_manager *$)  { $->pause_index = 0; }
void de_manager_reset(de_manager *$)   { $->pause_index = 0; de_manager_iterateAll($, de_entity_delete(ENTITY)); }
void de_manager_end(de_manager *$)     { free($->list); $->list = NULL; $->size = $->pause_index = 0; }