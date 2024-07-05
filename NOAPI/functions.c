#include "include.h"

void de_NOAPI_entity_destroy(de_entity *const this)
{
    if (this->state->leave != 0)
        this->state->leave(this, this->data);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}

void de_NOAPI_entity_update(de_entity *const this)
{
    if (this->ctrl == 2)
    {
        int index = uplist_find(this->manager, this);

        if (index >= 0)
        {
            de_NOAPI_entity_destroy(this->manager->items[index]);
            uplist_removeByIndex(this->manager, index);
        }

        return;
    }

    if (this->newState != 0)
    {
        de_state_leave(this);

        this->state = this->newState;
        this->newState = 0;

        de_state_enter(this);
    }

    if (this->ctrl == 0)
        this->state->update(this, this->data);
    else
        this->ctrl = 0;
}