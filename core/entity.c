#include "entity.h"
#include "../debug.h"

static void _nullf() {}

//

void de_entity_setState(de_entity *const this, de_state state)
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: state is null. Not set");
    }

    this->handler = state ?: _nullf;
}

void de_entity_setDestructor(de_entity *const this, de_state state)
{
    this->destructor = state ?: _nullf;
}
