#include "entity.h"
#include "../config.h"

#define _PROCESS(SET, MSG)         \
    if (this != 0)                 \
    {                              \
        SET;                       \
        return this;               \
    }                              \
    DARKEN_LOG("de_entity: " MSG); \
    return 0

//

de_state de_entity_exec(de_entity *const this)
{
    if (this == 0 || this->state == 0)
    {
        DARKEN_LOG("exec");
        return 0;
    }

    return this->state(this->data, this);
}

de_entity *de_entity_delete(de_entity *const this)
{
    _PROCESS(this->state = 0, "delete");
}

de_entity *de_entity_state(de_entity *const this, de_state_f state)
{
    _PROCESS(this->state = state, "state");
}

de_entity *de_entity_destructor(de_entity *const this, de_state_f destructor)
{
    _PROCESS(this->destructor = destructor, "destructor");
}
