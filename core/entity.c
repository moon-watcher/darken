#include "entity.h"
#include "manager.h"

static void swapIndex(de_entity *$, uint16_t swap)
{
    de_entity **entities = de_manager_getEntities($->manager);

    entities[$->index] = entities[swap];
    entities[$->index]->index = $->index;
    entities[swap] = $;
    $->index = swap;
}

void de_entity_state(de_entity *$, de_state state)
{
    $->state = state;
}

void de_entity_pause(de_entity *$)
{
    if (de_entity_isActive($))
        swapIndex($, $->manager->pause_index++);
}

void de_entity_resume(de_entity *$)
{
    if (de_entity_isPaused($))
        swapIndex($, --$->manager->pause_index);
}

void de_entity_delete(de_entity *$)
{
    if (de_entity_isDeleted($))
        return;

    de_entity_resume($);
    swapIndex($, --$->manager->list.size);

    $->destructor && $->destructor($->data);
}

uint16_t de_entity_isPaused(de_entity *$)
{
    return $->index < $->manager->pause_index;
}

uint16_t de_entity_isActive(de_entity *$)
{
    return $->index >= $->manager->pause_index && $->index < $->manager->list.size;
}

uint16_t de_entity_isDeleted(de_entity *$)
{
    return $->index >= $->manager->list.size;
}
