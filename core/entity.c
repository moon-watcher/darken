#include "entity.h"

void de_entity_set(de_entity *const this, de_state *const newState)
{
    this->newState = newState;
}

void de_entity_delay(de_entity *const this)
{
    this->delay = 1;
}

void de_entity_update(de_entity *const this)
{
    if (this->newState != 0)
    {
        de_state_leave(this);

        this->state = this->newState;
        this->newState = 0;

        de_state_enter(this);
    }

    if (this->delay == 0)
        this->state->update(this, this->data);
    else
        this->delay = 0;
}

void de_entity_destroy(de_entity *const this)
{
    if (this->state->leave != 0)
        this->state->leave(this, this->data);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}