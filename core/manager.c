#include "debug.h"
#include "manager.h"
#include "entity.h"
#include "entity.status.h"
#include "../config.h"

#define _DARKEN_MANAGER_EXEC(METHOD, ENTITY, DATA) \
    if (METHOD != 0)                               \
    {                                              \
        METHOD(ENTITY, DATA);                      \
    }

static void _nullf() {}
static const de_state _empty = {_nullf, _nullf, _nullf};

static void _destroy(de_entity *const this)
{
    _DARKEN_MANAGER_EXEC(this->leave, this, this->data);
    _DARKEN_MANAGER_EXEC(this->destructor, this, this->data);
}

//

static void _entity_update(de_entity *const this, void *const data)
{
    this->update(this, data);
}

static void _entity_skip(de_entity *const this, void *const data)
{
    this->status = _DARKEN_ENTITY_STATUS_UPDATE;
}

static void _entity_delete(de_entity *const this, void *const data)
{
#if DARKEN_DEBUG
    switch (uclist_remove(&this->manager->list, this, _destroy))
    {
    case -1:
        DARKEN_DEBUG_WARNING("manager, entity not found");
        break;
    case -2:
        DARKEN_DEBUG_WARNING("manager, this->count is 0");
    }
#else
    uclist_remove(&this->manager->list, this, _destroy);
#endif
}

static void _entity_set(de_entity *const this, void *const data)
{
    _DARKEN_MANAGER_EXEC(this->leave, this, data);

    de_state *const state = this->state ?: &_empty;

    _DARKEN_MANAGER_EXEC(state->enter, this, data);

    this->update = state->update ?: _nullf;
    this->leave = state->leave ?: _nullf;
    this->status = _DARKEN_ENTITY_STATUS_UPDATE;
}

static const void (*const _entity_array[_DARKEN_ENTITY_STATUS_MAX])(de_entity *const, void *const) = {
    [_DARKEN_ENTITY_STATUS_UPDATE] = _entity_update,
    [_DARKEN_ENTITY_STATUS_SKIP] = _entity_skip,
    [_DARKEN_ENTITY_STATUS_DELETE] = _entity_delete,
    [_DARKEN_ENTITY_STATUS_SET] = _entity_set};

//

void de_manager_loop(unsigned *const loop, de_state *const loop_state, unsigned size)
{
    size += sizeof(de_entity);
    de_entity *entity = malloc(size);
    memset(entity, 0, size);

    de_state *state = loop_state ?: &_empty;
    entity->update = state->update ?: _nullf;
    entity->leave = state->leave ?: _nullf;

    _DARKEN_MANAGER_EXEC(state->enter, entity, entity->data);

    while (*loop == 1)
    {
        _entity_array[entity->status](entity, entity->data);
    }

    _DARKEN_MANAGER_EXEC(entity->leave, entity, entity->data);

    free(entity);
    entity = 0;
}

void de_manager_init(de_manager *const this, unsigned bytes, unsigned datasize)
{
    uclist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    this->data = 0;

    if (datasize == 0)
    {
        DARKEN_DEBUG_NOTICE("manager datasize is 0");
    }
    else if ((this->data = malloc(datasize)) == 0)
    {
        DARKEN_DEBUG_ERROR("manager malloc() is null");
    }
}

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_DEBUG_ERROR("manager uclist_alloc() is null");
    }
    else
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
    uclist *const list = &this->list;
    unsigned const count = list->count;

    for (unsigned i = 0; i < count; i++)
    {
        de_entity *const entity = list->items[i];
        _entity_array[entity->status](entity, entity->data);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist *const list = &this->list;

    uclist_iterator(list, _destroy, 1);
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
}