#include "../core/entity.h"
#include "entity.h"

static void *_nullf() { return 0; }

////////////////////////////////////////////////////////////////

static void _update(de_entity *const this)
{
    de_state *newstate = this->update(this, this->data);
    ++this->timer;

    if (newstate != 0)
    {
        de_entity_set(this, newstate);
    }
}

static void _delete(de_entity *const this)
{
    _de_manager_delete(this->manager, this);
}

static void _transition(de_entity *const this)
{
    _EXEC(leave, this);

    this->state = this->state ?: &(de_state){_nullf, _nullf, _nullf};
    this->update = this->state->update ?: _nullf;
    this->event = 0;
    this->timer = 0;

    _EXEC(enter, this);
}

static const void (*const _entity_array[])(de_entity *const) = {_update, _delete, _transition};

////////////////////////////////////////////////////////////////

void _de_entity_init(de_entity *const this, de_manager *const manager, void (*desctructor)())
{
    this->update = _nullf;
    this->destructor = desctructor;
    this->manager = manager;
}

// DSM_update
void _de_entity_update(de_entity *const this)
{
    _entity_array[this->event](this);
}

// DSM_transition


// DSM ????
void _de_entity_destroy(de_entity *const this)
{
    _EXEC(leave, this);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}
