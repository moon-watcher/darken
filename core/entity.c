#include "entity.h"
#include "manager.h"

void de_entity_state(de_entity *$, de_state state)
{
    $->state = state;
}

uint16_t de_entity_pause(de_entity *$)
{
    if (de_entity_isPaused($))
        return 0;

    de_manager *manager = $->manager;
    de_entity **items = manager->list.items;
    uint16_t pause_index = manager->pause_index++;

    items[$->index] = items[pause_index];
    items[$->index]->index = $->index;
    items[pause_index] = $;
    $->index = pause_index;

    return 1;
}

uint16_t de_entity_resume(de_entity *$)
{
    if (!de_entity_isPaused($))
        return 0;

    de_manager *manager = $->manager;
    de_entity **items = manager->list.items;
    uint16_t pause_index = --manager->pause_index;

    items[$->index] = items[pause_index];
    items[$->index]->index = $->index;
    items[pause_index] = $;
    $->index = pause_index;

    return 1;
}

uint16_t de_entity_delete(de_entity *$)
{
    if (de_entity_isDeleted($))
        return 0;

    de_manager *manager = $->manager;
    de_entity **items = manager->list.items;
    uint16_t size = --manager->list.size;

    items[$->index] = items[size];

    if ($->index < size)
        items[$->index]->index = $->index;

    if ($->destructor)
        $->destructor($->data);

    return 1;
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
