#include "entity.h"

de_entity *de_entity_set(de_entity *const this, de_state (*state)())
{
    this->handler = (state != 0) ? state : ({de_state f(){ return f; } f; });

    return this;
}

de_entity *de_entity_destructor(de_entity *const this, de_state (*destructor)())
{
    this->destructor = destructor;

    return this;
}

//

inline __attribute__((always_inline)) de_state de_entity_exec(de_entity *const this)
{
    return this->handler(this->data, this);
}

inline __attribute__((always_inline)) de_state de_entity_update(de_entity *const this)
{
    return this->handler = de_entity_exec(this);
}

de_state de_entity_delete(de_entity *const this)
{
    return (this->destructor != 0) ? this->destructor(this->data, this) : 0;
}
