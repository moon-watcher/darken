#include "config.h"
#include "core/entity.h"
#include "core/manager.h"
#include "debug.h"

static bool loop;

void darken_loop(de_state state, unsigned size)
{
    if (state == 0)
    {
        return;
    }

    de_entity *entity = malloc(sizeof(de_entity) + size);
    de_entity_setState(entity, state);

    loop = true;
    while (loop == true)
    {
        de_entity_update(entity);
    }

    free(entity);
}

void darken_break()
{
    loop = false;
}



void de_manager_init(de_manager *const this, unsigned bytes)
{
    de_manager_initialize(this, sizeof(de_entity) + bytes, de_entity_update, de_entity_destroy);
}

de_entity *de_manager_newEntity(de_manager *const this)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity == 0)
    {
        DARKEN_ERROR("Manager: uclist_alloc() is null");
        return 0;
    }

    entity->manager = this;

    return entity;
}

int de_manager_deleteEntity(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, de_entity_destroy);

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
