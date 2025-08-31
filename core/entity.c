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
    return de_entity_set(this, 0);
}

inline de_entity *de_entity_pause(de_entity *const this)
{
    return de_entity_set(this, de_state_empty);
}

inline de_entity *de_entity_update(de_entity *const this)
{
    return de_entity_set(this, this->state(this->data));
}
