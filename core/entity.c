#include "entity.h"
#include "../config.h"

#define _DARKEN_LOG(MSG)               \
    else                               \
    {                                  \
        DARKEN_LOG("de_entity: " MSG); \
    }

#define _PROCESS(THIS, ASSIGN, MSG) ({ \
    if (THIS != 0)                     \
    {                                  \
        ASSIGN;                        \
    }                                  \
    _DARKEN_LOG(MSG);                  \
    THIS; })

//

de_state de_entity_exec(de_entity *const this)
{
    if (this != 0)
    {
        de_state_f state = this->state;

        if (state != 0)
        {
            return state(this->data, this);
        }
        _DARKEN_LOG("exec, state");
    }
    _DARKEN_LOG("exec, this");

    return 0;
}

de_entity *de_entity_delete(de_entity *const this)
{
    return _PROCESS(this, this->state = 0, "delete");
}

de_entity *de_entity_state(de_entity *const this, de_state_f state)
{
    return _PROCESS(this, this->state = state, "state");
}

de_entity *de_entity_destructor(de_entity *const this, de_state_f destructor)
{
    return _PROCESS(this, this->destructor = destructor, "destructor");
}
