#include "entity.h"

de_entity *de_entity_set(de_entity *const this, de_state_f state)
{
    this->state = (state != 0) ? state : de_state_null;

    return this;
}

de_entity *de_entity_destructor(de_entity *const this, de_state_f destructor)
{
    this->destructor = destructor;

    return this;
}

//

inline __attribute__((always_inline)) de_state de_entity_exec(de_entity *const this)
{
    return this->state(this->data, this);
}

inline __attribute__((always_inline)) de_state de_entity_update(de_entity *const this)
{
    return this->state = de_entity_exec(this);
}

de_state de_entity_delete(de_entity *const this)
{
    return (this->destructor != 0) ? this->destructor(this->data, this) : 0;
}
