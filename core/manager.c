#include "manager.h"

void de_manager_init(de_manager *$, de_manager_def *def, void *)
{
    // $->list = buffer;
    $->itemSize = def->itemSize;
    $->capacity = def->capacity;
    $->size = 0;
    $->pause_index = 0;
}

de_entity *de_manager_new(de_manager *$)
{
    if ($->size >= $->capacity) return NULL;

    de_entity *e = (uint8_t *)$->list + $->size * $->itemSize;
    memset(e, 0, $->itemSize);

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

void de_manager_pause(de_manager *$) { $->pause_index = $->size; }
void de_manager_resume(de_manager *$) { $->pause_index = 0; }
void de_manager_reset(de_manager *$)
{
    de_manager_resume($);
    de_manager_iterate($, de_entity_delete(ENTITY));
}
void de_manager_end(de_manager *$) { $->size = $->pause_index = 0; }