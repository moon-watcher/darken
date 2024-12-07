#include "entity.h"

de_state de_entity_exec(de_entity *const this)
{
    return (this->state != 0) ? this->state(this->data, this) : 0;
}

void de_entity_delete(de_entity *const this)
{
    this->state = 0;
}
