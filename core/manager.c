#include "manager.h"
#include "entity.h"
#include "../private/entity.h"
#include "../private/state.h"
#include "../config.h"

static void _destroy(de_entity *const this)
{
    if (this->leave != 0)
    {
        this->leave(this, this->data);
    }

    if (this->destructor != 0)
    {
        this->destructor(this, this->data);
    }
}

static void _update(de_entity *const this)
{
    this->update(this, this->data);
}

static void _skip(de_entity *const this)
{
    this->status = _DE_ENTITY_STATUS_UPDATE;
}

static void _delete(de_entity *const this)
{
    int ret = uclist_remove(&this->manager->list, this, _destroy);

#if DARKEN_DEBUG
    switch (ret)
    {
    case -1:
        DARKEN_WARNING("manager, entity not found");
        break;
    case -2:
        DARKEN_WARNING("manager, this->count");
        break;
    default:
        DARKEN_INFO("manager _delete");
        break;
    }
#endif
}

static void _set(de_entity *const this)
{
    if (this->leave != 0)
    {
        this->leave(this, this->data);
    }

    de_state *const state = this->state;

    if (state->enter != 0)
    {
        state->enter(this, this->data);
    }

    this->update = state->update ?: _de_state_nullf;
    this->leave = state->leave ?: _de_state_nullf;
    this->status = _DE_ENTITY_STATUS_UPDATE;
}

static const void (*const _array[_DE_ENTITY_STATUS_MAX])(de_entity *const) = {
    [_DE_ENTITY_STATUS_UPDATE] = _update,
    [_DE_ENTITY_STATUS_SKIP] = _skip,
    [_DE_ENTITY_STATUS_DELETE] = _delete,
    [_DE_ENTITY_STATUS_SET] = _set};

//

void de_manager_loop(unsigned *const loop, de_state *const loop_state, unsigned size)
{
    size += sizeof(de_entity);
    de_entity *entity = malloc(size);
    memset(entity, 0, size);

    de_state *state = loop_state ?: &_de_state_empty;
    entity->update = state->update ?: _de_state_nullf;
    entity->leave = state->leave ?: _de_state_nullf;

    if (state->enter != 0)
    {
        state->enter(entity, entity->data);
    }

    while (*loop == 1)
    {
        _array[entity->status](entity);
    }

    if (entity->leave != 0)
    {
        entity->leave(entity, entity->data);
    }

    free(entity);
    entity = 0;
}

void de_manager_init(de_manager *const this, unsigned bytes, unsigned datasize)
{
    uclist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    this->data = 0;

    if (datasize == 0)
    {
        DARKEN_NOTICE("manager datasize is 0");
    }
    else if ((this->data = malloc(datasize)) == 0)
    {
        DARKEN_ERROR("manager malloc() is null");
    }
    else
    {
        DARKEN_INFO("manager init");
    }
}

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_ERROR("manager uclist_alloc() is null");
    }
    else
    {
        entity->update = _de_state_nullf;
        entity->leave = _de_state_nullf;
        entity->destructor = desctructor ?: _de_state_nullf;
        entity->manager = this;

        DARKEN_INFO("manager added entity");
    }

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uclist *const list = &this->list;
    unsigned const count = list->count;

    for (unsigned i = 0; i < count; i++)
    {
        de_entity *const entity = list->items[i];
        _array[entity->status](entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist *const list = &this->list;
    unsigned const count = list->count;

    DARKEN_INFO("manager reset");

    for (unsigned i = 0; i < count; i++)
    {
        _destroy(list->items[i]);
    }

    uclist_reset(list);
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
    uclist_end(&this->list);

    free(this->data);
    this->data = 0;

    memset(this, 0, sizeof(de_manager));

    DARKEN_INFO("manager end");
}