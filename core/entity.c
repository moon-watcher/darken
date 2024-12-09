#include "entity.h"

de_state de_entity_exec(de_entity *const this)
{
    if (this->state == 0)
    {
        return 0;
    }

    return this->state(this->data, this);
}

void de_entity_delete(de_entity *const this)
{
    this->state = 0;
}
