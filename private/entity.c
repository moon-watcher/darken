#include "entity.h"

__attribute__((always_inline)) inline void dep_entity_update(de_entity *const this)
{
    this->update(this);
}

__attribute__((always_inline)) inline void dep_entity_destruct(de_entity *const this)
{
#include "xtor.h"
#include "state.h"
    dep_state_leave(this);
    dep_xtor_leave(this);
}