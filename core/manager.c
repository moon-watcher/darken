#include "manager.h"
#include "entity.h"
#include "../config.h"

static void _destroy(de_entity *const this)
{
    if (this->leave != 0)
        this->leave(this, this->data);

    if (this->destructor != 0)
        this->destructor(this, this->data);
}

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
    if (uplist_remove(&this->manager->list, this, _destroy) == 0)
        LOG("WARNING: Not found or count is 0");
}

static void _set(de_entity *const this)
{
    this->leave(this, this->data);

    de_state *const state = this->state;

    if (state->enter != 0)
        state->enter(this, this->data);

    this->update = state->update ?: de_state_nullf;
    this->leave = state->leave ?: de_state_nullf;
    this->ctrl = 0;
}

const static void (*const _array[])(de_entity *const) = {_update, _delay, _delete, _set};

//

void de_manager_loop(unsigned *const loop, de_state *state, unsigned size)
{
    size += sizeof(de_entity);
    de_entity *entity = malloc(size);
    memset(entity, 0, size);

    state = state ?: &de_state_empty;
    entity->update = state->update ?: de_state_nullf;
    entity->leave = state->leave ?: de_state_nullf;

    if (state->enter != 0)
        state->enter(entity, entity->data);

    while (*loop == 1)
        _array[entity->ctrl](entity);

    entity->leave(entity, entity->data);

    free(entity);
}

void de_manager_init(de_manager *const this, unsigned bytes, unsigned datasize)
{
    uplist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    this->data = 0;

    if (datasize == 0)
    {
        // LOG("NOTICE: datasize is 0");
    }
    else if ((this->data = malloc(datasize)) == 0)
    {
        LOG("ERROR: malloc() returns null");
    }
}

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uplist_alloc(&this->list);

    if (entity == 0)
    {
        LOG("ERROR: uplist_alloc() returns null entity");
    }
    else
    {
        entity->update = de_state_nullf;
        entity->leave = de_state_nullf;
        entity->destructor = desctructor ?: de_state_nullf;
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
        _array[entity->ctrl](entity);
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

unsigned de_manager_count(de_manager *const this)
{
    return this->list.count;
}

unsigned de_manager_capacity(de_manager *const this)
{
    return this->list.capacity;
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(&this->list);

    if (this->data != 0)
        free(this->data);

    memset(this, 0, sizeof(de_manager));
}