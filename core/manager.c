#include "manager.h"

static void _nullf() {}
static const de_state _emptystate = {_nullf, _nullf, _nullf};

//

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_createEntity(de_manager *const this, de_state_f desctructor)
{
    de_entity *entity = uplist_alloc(this);

    if (entity == 0)
        return 0;

    entity->state = (de_state *)&_emptystate;
    entity->destructor = desctructor;

    return entity;
}

int de_manager_deleteEntity(de_manager *const this, de_entity *entity)
{
    if (entity == 0)
        return -2;

    int index = uplist_find(this, entity);

    if (index < 0)
        return -1;

    de_entity_destroy(this->items[index]);

    return uplist_remove(this, entity);
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(this, de_entity_update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(this, de_entity_destroy, 1);
    uplist_reset(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(this);
}