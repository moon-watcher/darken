#include "debug.h"
#include "manager.h"
#include "entity.h"
#include "../priv/entity.h"
#include "../priv/common.h"
#include "../config.h"

#define _EXEC(METHOD, ENTITY, DATA)          \
    if (ENTITY->state->METHOD != 0)          \
    {                                        \
        ENTITY->state->METHOD(ENTITY, DATA); \
    }

#define _TICK(ENTITY) \
    _entity_array[ENTITY->status](ENTITY, ENTITY->data)

static void _nullf() {}

static void _destroy(de_entity *const this)
{
    _EXEC(leave, this, this->data);

    this->destructor(this, this->data);
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
    _EXEC(leave, this, data);

    this->state = this->state ?: &(de_state){_nullf, _nullf, _nullf};
    this->update = this->state->update ?: _nullf;
    this->status = _DARKEN_ENTITY_STATUS_UPDATE;

    _EXEC(enter, this, data);
}

static const void (*const _entity_array[_DARKEN_ENTITY_STATUS_MAX])(de_entity *const, void *const) = {
    [_DARKEN_ENTITY_STATUS_UPDATE] = _entity_update,
    [_DARKEN_ENTITY_STATUS_SKIP] = _entity_skip,
    [_DARKEN_ENTITY_STATUS_DELETE] = _entity_delete,
    [_DARKEN_ENTITY_STATUS_SET] = _entity_set};

//

void de_manager_loop(unsigned *const loop, de_state *const state, unsigned size)
{
    de_entity *entity = malloc(size + sizeof(de_entity));

    entity->state = state;
    _entity_set(entity, entity->data);

    while (*loop == 1)
    {
        _TICK(entity);
    }

    _EXEC(leave, entity, entity->data);

    free(entity);
}

void de_manager_init(de_manager *const this, unsigned bytes, unsigned datasize)
{
    uclist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    _DARKEN_COMMON_INIT(this, datasize);
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
        _TICK(entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(&this->list, _destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    _DARKEN_COMMON_END(this, de_manager);
}