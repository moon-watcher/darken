#include "entity.h"
#include "../NOAPI/state.h"

static void update(de_entity *const entity)
{
    entity->update(entity, entity->data);
}

static void delay(de_entity *const entity)
{
    // dalay
}

static void delete(de_entity *const entity)
{
    uplist_remove(&entity->manager->list, entity, de_NOAPI_entity_destroy);
}

static void set(de_entity *const entity)
{
    entity->leave(entity, entity->data);

    de_state *const state = entity->state;

    if (state->enter != 0)
        state->enter(entity, entity->data);

    entity->update = state->update ?: de_NOAPI_state_nullf;
    entity->leave = state->leave ?: de_NOAPI_state_nullf;
    entity->ctrl = 0;
}

static void (*const functions[])(de_entity *const) = {update, delay, delete, set};

//

void de_NOAPI_entity_destroy(de_entity *const this)
{
    if (this->leave != 0)
        this->leave(this, this->data);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}

void de_NOAPI_entity_update(de_entity *const this)
{
    functions[this->ctrl](this);
}