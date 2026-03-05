#include "entity.h"
#include "manager.h"

void de_entity_delete(de_entity *$)
{
    uclist_remove(&$->manager->list, $);
    $->destructor && $->destructor($->data);
    $->state = 0;
}
