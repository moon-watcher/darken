#include "entity.h"
#include "../NOAPI/state.h"

static void _entity_update(de_entity *const entity);
static void _entity_delay(de_entity *const entity);
static void _entity_delete(de_entity *const entity);
static void _entity_set(de_entity *const entity);

static void (*const _update_entity[])(de_entity *const) = {_entity_update, _entity_delay, _entity_delete, _entity_set};

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
    _update_entity[this->ctrl](this);
}

//

static void _entity_update(de_entity *const entity)
{
    entity->update(entity, entity->data);
}

static void _entity_delay(de_entity *const entity)
{
    // dalay
}

static void _entity_delete(de_entity *const entity)
{
    uplist *const list = &entity->manager->list;

    int index = uplist_find(list, entity);

    if (index < 0)
        return;

    de_NOAPI_entity_destroy(list->items[index]);
    uplist_removeByIndex(list, index);
}

static void _entity_set(de_entity *const entity)
{
    entity->leave(entity, entity->data);

    de_state *const state = entity->state;

    if (state->enter != 0)
        state->enter(entity, entity->data);

    entity->update = state->update ?: de_NOAPI_state_nullf;
    entity->leave = state->leave ?: de_NOAPI_state_nullf;
    entity->ctrl = 0;
}