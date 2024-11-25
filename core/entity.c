#include "entity.h"
#include "manager.h"

int de_entity_delete(de_entity *const this)
{
    return de_manager_delete(this->manager, this);
}
