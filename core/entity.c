#include "entity.h"
#include "../common/include.h"

void de_entity_set(de_entity *const this, de_state *const newState)
{
    this->newState = newState;
}

void de_entity_delay(de_entity *const this)
{
    this->delay = 1;
}

int de_entity_delete(de_entity *const this)
{
    if (this == 0)
        return -2;

    int index = uplist_find(this->manager, this);

    if (index < 0)
        return -1;

    de_noAPI_entity_destroy(this->manager->items[index]);

    uplist_removeByIndex(this->manager, index);

    return index;
}