#include "entity.h"

enum
{
    EVENT_DELETE = 1,
    EVENT_TRANSACTION
};

void de_entity_set(de_entity *const this, de_state *state)
{
    this->state = state;
    this->event = EVENT_TRANSACTION;
    // dsm_trans_set();
}

void de_entity_delete(de_entity *const this)
{
    this->event = EVENT_DELETE;
    // dsm_trans_delete();
}

// DSM: Darkula State Machine
