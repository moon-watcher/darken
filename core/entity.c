#include "entity.h"

void *de_entity_delete(de_entity *const this)
{
    return this->destructor ? this->destructor(this->data) : 0;
}
