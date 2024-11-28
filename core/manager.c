#include "manager.h"
#include "../debug.h"

static inline __attribute__((always_inline)) void _update(de_entity *const entity)
{
    entity->handler = entity->handler(entity->data, entity);
}

static void _destroy(de_entity *const entity)
{
    entity->destructor != 0 && entity->destructor(entity->data, entity);
}

static void *_nullf()
{
    return _nullf;
}

//

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(&this->list, sizeof(de_entity) + bytes);
}

void de_manager_update(de_manager *const this)
{
    uclist_iterator(&this->list, _update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(&this->list, _destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uclist_end(&this->list);
}

//

de_entity *de_manager_new(de_manager *const this)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_ERROR("Manager: uclist_alloc() is null");
        return 0;
    }

    entity->handler = _nullf;

    return entity;
}

int de_manager_delete(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, _destroy);

#if DARKEN_WARNING
    switch (ret)
    {
    case -1:
        DARKEN_WARNING("Manager: deleteEntity: entity not found");
        break;
    case -2:
        DARKEN_WARNING("Manager: deleteEntity: count is 0");
        break;
    }
#endif

    return ret;
}
