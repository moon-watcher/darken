#include "entity.h"

inline de_entity *de_entity_set(de_entity *const this, de_state state)
{
    return this->state = state, this;
}

inline de_entity *de_entity_destructor(de_entity *const this, de_state state)
{
    return this->destructor = state, this;
}

inline de_entity *de_entity_delete(de_entity *const this)
{
    return this->state = 0, this;
}

inline de_entity *de_entity_pause(de_entity *const this)
{
    return this->state = de_state_empty, this;
}

inline de_entity *de_entity_update(de_entity *const this)
{
    return this->state = this->state(this->data), this;
}
