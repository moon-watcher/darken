#include "../core/entity.h"
#include "entity.h"

void _de_entity_init(de_entity *const this, de_manager *const manager, de_state desctructor)
{
    this->state = ({ void *f() { return 0; } f; });
    this->destructor = desctructor;
    this->manager = manager;
}

void _de_entity_update(de_entity *const this)
{
    void *state = this->state(this, this->data);
    ++this->timer;

    if (state != 0)
    {
        this->state = state;
        this->timer = 0;
    }
}

void _de_entity_destroy(de_entity *const this)
{
    if (this->destructor != 0)
        this->destructor(this, this->data);
}
