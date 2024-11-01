#include "entity.h"
#include "../debug.h"

static void _nullf() {}

//

void de_entity_setState(de_entity *const this, void (*state)())
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: state is null. Not set");
    }

    this->handler = state ?: _nullf;
}

void de_entity_setDestructor(de_entity *const this, void (*destructor)())
{
    if (destructor == 0)
    {
        DARKEN_ERROR("Entity: destructor is null. Not set");
    }

    this->destructor = destructor ?: _nullf;
}
