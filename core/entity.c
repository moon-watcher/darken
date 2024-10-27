#include "entity.h"
#include "../debug.h"

static void _nullf()
{
    //
}

//

void *de_entity_set(de_entity *const this, void (*state)())
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: null state");
        this->state = _nullf;
        return 0;
    }

    return this->state = state;
}

void *de_entity_destructor(de_entity *const this, void (*destructor)())
{
    if (destructor == 0)
    {
        this->destructor = _nullf;
        return 0;
    }

    return this->destructor = destructor;
}

void *de_entity_exec(de_entity *const this, void (*exec)())
{
    if (exec == 0)
    {
        return 0;
    }

    exec(this, this->data);

    return exec;
}
