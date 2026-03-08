#include "entity.h"
#include "manager.h"

void de_entity_state(de_entity *$, de_state state)
{
    $->state = state;
}

void de_entity_pause(de_entity *$)
{
    //
}

int16_t de_entity_resume(de_entity *entity)
{
    de_manager *manager = entity->manager;

    de_entity **items = manager->list.items;
    uint16_t i = manager->pause_index;

    while (i--)
    {
        if (items[i] == entity)
        {
            items[i] = items[--manager->pause_index];
            items[manager->pause_index] = entity;
            return i;
        }
    }

    return -1;
}

void de_entity_delete(de_entity *$)
{
    uclist_remove(&$->manager->list, $);
    $->destructor && $->destructor($->data);
}
