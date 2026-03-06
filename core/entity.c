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

void de_entity_resume(de_entity *$)
{
    //
}

void de_entity_delete(de_entity *$)
{
    uclist_remove(&$->manager->list, $);
    $->destructor && $->destructor($->data);
}
