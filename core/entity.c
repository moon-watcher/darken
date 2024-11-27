#include "entity.h"

void *de_entity_delete(de_entity *const this)
{
    if (this->destructor == 0)
    {
        return 0;
    }

    return this->destructor(this, this->data);
}
