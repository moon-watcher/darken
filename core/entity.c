#include "entity.h"

void de_entity_set(de_entity *const this, de_state *state)
{
    _de_entity_transtion(this, state);
    // this->state = state;
    // this->event = EVENT_TRANSACTION;
    // dsm_trans_set();
}

void de_entity_delete(de_entity *const this)
{
    _de_manager_delete(this->manager, this);
    
    // this->event = EVENT_DELETE;
    // dsm_trans_delete();
}

// DSM: Darkula State Machine
