#include "entity.h"
#include "../config.h"

de_state de_entity_exec(de_entity *const this)
{
    if (this != 0 && this->state != 0)
    {
        return this->state(this->data, this);
    }

    DARKEN_LOG("de_entity: exec");
    return 0;
}

de_entity *de_entity_delete(de_entity *const this)
{
    if (this != 0)
    {
        this->state = 0;
        return this;
    }

    DARKEN_LOG("de_entity: delete");
    return 0;
}

de_entity *de_entity_state(de_entity *const this, de_state_f state)
{
    if (this != 0)
    {
        this->state = state;
        return this;
    }

    DARKEN_LOG("de_entity: state");
    return 0;
}

de_entity *de_entity_destructor(de_entity *const this, de_state_f destructor)
{
    if (this != 0)
    {
        this->destructor = destructor;
        return this;
    }

    DARKEN_LOG("de_entity: destructor");
    return 0;
}
