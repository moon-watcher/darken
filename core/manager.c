#include "manager.h"
#include "entity.h"

static void _update(de_entity *const);
static void _delay(de_entity *const);
static void _delete(de_entity *const);
static void _set(de_entity *const);
static void _destroy(de_entity *const);
static void _nullf();
static void (*const _entity_functions[])(de_entity *const) = {_update, _delay, _delete, _set};

//

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(&this->list, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uplist_alloc(&this->list);

    if (entity != 0)
    {
        entity->update = _nullf;
        entity->leave = _nullf;
        entity->destructor = desctructor ?: _nullf;
        entity->manager = this;
    }

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uplist *const list = &this->list;
    unsigned const count = list->count;

    for (unsigned i = 0; i < count; i++)
    {
        de_entity *const entity = list->items[i];
        _entity_functions[entity->ctrl](entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uplist *const list = &this->list;
    unsigned const count = list->count;

    for (unsigned i = 0; i < count; i++)
        _destroy(list->items[i]);

    uplist_reset(list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);

    uplist_end(&this->list);
}

//

static void _update(de_entity *const entity)
{
    entity->update(entity, entity->data);
}

static void _delay(de_entity *const entity)
{
    // dalay
}

static void _delete(de_entity *const entity)
{
    uplist_remove(&entity->manager->list, entity, _destroy);
}

static void _set(de_entity *const entity)
{
    entity->leave(entity, entity->data);

    de_state *const state = entity->state;

    if (state->enter != 0)
        state->enter(entity, entity->data);

    entity->update = state->update ?: _nullf;
    entity->leave = state->leave ?: _nullf;
    entity->ctrl = 0;
}

static void _destroy(de_entity *const this)
{
    if (this->leave != 0)
        this->leave(this, this->data);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}

static void _nullf()
{
    //
}