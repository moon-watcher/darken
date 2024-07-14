#include "entity.h"
#include "state.h"
#include "../config.h"

static void _update(de_entity *const);
static void _delay(de_entity *const);
static void _delete(de_entity *const);
static void _set(de_entity *const);

//

void (*const de_NOAPI_entity_array[])(de_entity *const) = {_update, _delay, _delete, _set};

void de_NOAPI_entity_destroy(de_entity *const this)
{
    if (this->leave != 0)
        this->leave(this, this->data);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}

//

static void _update(de_entity *const this)
{
    this->update(this, this->data);
}

static void _delay(de_entity *const this)
{
    // dalay
}

static void _delete(de_entity *const this)
{
    // uplist_remove(&this->manager->list, this, de_NOAPI_entity_destroy);
    
    if (uplist_remove(&this->manager->list, this, de_NOAPI_entity_destroy) == 0)
        LOG("WARNING: Not found or count is 0");
}

static void _set(de_entity *const this)
{
    this->leave(this, this->data);

    de_state *const state = this->state;

    if (state->enter != 0)
        state->enter(this, this->data);

    this->update = state->update ?: de_NOAPI_state_nullf;
    this->leave = state->leave ?: de_NOAPI_state_nullf;
    this->ctrl = 0;
}