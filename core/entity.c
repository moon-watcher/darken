#include "entity.h"
#include "../config.h"

enum
{
    STATUS_UPDATE,
    STATUS_DELETE,
    STATUS_SET
};


void de_entity_set(de_entity *const this, de_state *state)
{
    this->state = state;
    this->status = STATUS_SET;
}

void de_entity_delete(de_entity *const this)
{
    this->status = STATUS_DELETE;
}
