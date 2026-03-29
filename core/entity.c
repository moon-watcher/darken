#include "entity.h"
#include "manager.h"

static void swapIndex(de_entity *$, uint16_t swap)
{
    uint16_t index = $->index;

    if (index == swap)
        return;

    de_entity **entities = $->manager->list.items;

    entities[index] = entities[swap];
    entities[index]->index = index;
    entities[swap] = $;
    $->index = swap;
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

    de_entity_resume($);
    swapIndex($, --$->manager->list.size);

    if ($->destructor)
        $->destructor($->data);
}
