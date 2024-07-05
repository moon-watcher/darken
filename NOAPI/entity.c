#include "entity.h"
#include "../NOAPI/state.h"

void de_NOAPI_entity_destroy(de_entity *const this)
{
    if (this->leave != 0)
        this->leave(this, this->data);

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
        this->leave(this, this->data);

        if (this->newState->enter != 0)
            this->newState->enter(this, this->data);

        this->update = this->newState->update ?: de_NOAPI_state_nullf;
        this->leave = this->newState->leave ?: de_NOAPI_state_nullf;

        this->newState = 0;
    }

    if (this->ctrl == 0)
        this->update(this, this->data);
    else
        this->ctrl = 0;
}