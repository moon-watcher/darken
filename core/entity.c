#include "entity.h"
#include "../config.h"

de_state de_entity_exec(de_entity *const this)
{
    if (this != 0)
    {
        de_state_f state = this->state;

        if (state != 0)
        {
            return state(this->data, this);
        }

        DARKEN_LOG("de_entity: exec, state");
    }
    else
    {
        DARKEN_LOG("de_entity: exec, this");
    }

    return 0;
}

de_entity *de_entity_delete(de_entity *const this)
{
    if (this != 0)
    {
        this->state = 0;
    }
    else
    {
        DARKEN_LOG("de_entity: delete");
    }

    return this;
}

de_entity *de_entity_state(de_entity *const this, de_state_f state)
{
    if (this != 0)
    {
        this->state = state;
    }
    else
    {
        DARKEN_LOG("de_entity: state");
    }

    return this;
}

de_entity *de_entity_destructor(de_entity *const this, de_state_f destructor)
{
    if (this != 0)
    {
        this->destructor = destructor;
    }
    else
    {
        DARKEN_LOG("de_entity: destructor");
    }

    return this;
}
