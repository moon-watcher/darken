#include "entity.h"
#include "manager.h"

de_entity *getEntity(de_manager *mgr, uint16_t index)
{
    return (de_entity *)((uint8_t *)mgr->list + index * mgr->itemSize);
}

static void swapIndex(de_entity *$, uint16_t swap)
{
    uint16_t index = $->index;
    if (index == swap) return;

    de_manager *mgr = $->manager;
    uint8_t *a = (uint8_t *)getEntity(mgr, index);
    uint8_t *b = (uint8_t *)getEntity(mgr, swap);

    // Temporal para swap
    uint8_t tmp[mgr->itemSize];
    memcpy(tmp, a, mgr->itemSize);
    memcpy(a, b, mgr->itemSize);
    memcpy(b, tmp, mgr->itemSize);

    // Actualizamos índices de las entidades
    ((de_entity *)a)->index = index;
    ((de_entity *)b)->index = swap;
}

void de_entity_pause(de_entity *$)
{
    if (de_entity_is_active($))
        swapIndex($, $->manager->pause_index++);
}

void de_entity_resume(de_entity *$)
{
    if (de_entity_is_paused($))
        swapIndex($, --$->manager->pause_index);
}

void de_entity_delete(de_entity *$)
{
    if (de_entity_is_deleted($))
        return;

    $->state = 0;
    de_entity_resume($);
    swapIndex($, --$->manager->size);

    if ($->destructor)
        $->destructor($->data);
}