#include "manager.h"
#include "../NOAPI/include.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uplist_alloc(this);

    if (entity == 0)
        return 0;

    void _nullf() {}
    const de_state _emptystate = {_nullf, _nullf, _nullf};

    entity->state = (de_state *)&_emptystate;
    entity->destructor = desctructor;
    entity->manager = this;

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(this, de_NOAPI_entity_update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(this, de_NOAPI_entity_destroy, 1);
    uplist_reset(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(this);
}