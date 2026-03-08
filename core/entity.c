#include "entity.h"
#include "manager.h"

void de_entity_state(de_entity *$, de_state state)
{
    $->state = state;
}

void de_entity_pause(de_entity *$)
{
    // TODO: pause entity in manager
    // Use ->manager & ->index
}

void de_entity_resume(de_entity *$)
{
    // TODO: resume entity in manager
    // Use ->manager & ->index
}

void de_entity_delete(de_entity *$)
{
    // TODO. use ->index to remove entity from manager
    uclist_remove(&$->manager->list, $);
    $->destructor && $->destructor($->data);
}

uint16_t de_entity_isPaused(de_entity *$)
{
    // TODO: check if entity is paused 
    // ->index < ->manager->pause_index
    return 0;
}

uint16_t de_entity_isActive(de_entity *$)
{
    // TODO: check if entity is active
    // ->index >= ->manager->pause_index && !de_entity_isDeleted($)
    return 0;
}

uint16_t de_entity_isDeleted(de_entity *$)
{
    // TODO: check if entity is deleted
    // ->index >= ->manager->list.size
    return 0;
}
