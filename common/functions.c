#include "include.h"

void de_noAPI_entity_destroy(de_entity *const this)
{
    if (this->state->leave != 0)
        this->state->leave(this, this->data);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}

void de_noAPI_entity_update(de_entity *const this)
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