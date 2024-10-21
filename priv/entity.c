#include "../core/entity.h"
#include "entity.h"

static void *_nullf() { return 0; }

enum
{
    EVENT_UPDATE,
    EVENT_TRANSACTION,
    EVEMT_MAX
};

////////////////////////////////////////////////////////////////

static void _update(de_entity *const this)
{
    de_state *newstate = this->handle(this, this->data);
    ++this->timer;

    if (newstate != 0)
    {
        _de_entity_transtion(this, newstate);
    }
}

static void _transition(de_entity *const this)
{
    _EXEC(leave, this);

    this->state = this->state ?: &(de_state){_nullf, _nullf, _nullf};
    this->handle = this->state->update ?: _nullf;
    this->event = EVENT_UPDATE;
    this->timer = 0;

    _EXEC(enter, this);
}

static const void (*const _entity_array[EVEMT_MAX])(de_entity *const) = {
    [EVENT_UPDATE] = _update,
    [EVENT_TRANSACTION] = _transition
    };

////////////////////////////////////////////////////////////////

// DSM_update & for Darken use
void _de_entity_update(de_entity *const this)
{
    _entity_array[this->event](this);
}

// DSM_transition
void _de_entity_transtion(de_entity *const this, de_state *const state)
{
    this->state = state;
    this->event = EVENT_TRANSACTION;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// for Darken use
void _de_entity_init(de_entity *const this, de_manager *const manager, void (*desctructor)())
{
    this->handle = _nullf;
    this->destructor = desctructor;
    this->manager = manager;
}

// for Darken use
void _de_entity_destroy(de_entity *const this)
{
    _EXEC(leave, this);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}
