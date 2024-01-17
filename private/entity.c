#include "entity.h"
#include "xtor.h"
#include "state.h"

void dep_entity_update(de_entity *const this)
{
    this->update(this);
}

void dep_entity_destruct(de_entity *const this)
{
    de_state_leave(this);
    de_xtor_leave(this);
}