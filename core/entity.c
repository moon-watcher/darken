#include "entity.h"
#include "manager.h"
#include "../uclist/uclist.h"

void de_entity_delete(de_entity *$)
{
    uclist_remove(&$->manager->list, $);
    $->destructor && $->destructor($->data);
}
