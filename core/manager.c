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
    uplist_remove(&this->manager->list, this, _destroy);
}

static void _set(de_entity *const this)
{
    this->leave(this, this->data);

    de_state *const state = this->state;

    if (state->enter != 0)
        state->enter(this, this->data);

    this->update = state->update ?: _nullf;
    this->leave = state->leave ?: _nullf;
    this->ctrl = 0;
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