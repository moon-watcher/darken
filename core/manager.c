#include "manager.h"
#include "../NOAPI/entity.h"
#include "../NOAPI/state.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(&this->list, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uplist_alloc(&this->list);

    if (entity == 0)
        return 0;

    entity->update = de_NOAPI_state_nullf;
    entity->leave = de_NOAPI_state_nullf;
    entity->destructor = desctructor ?: de_NOAPI_state_nullf;
    entity->manager = this;

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(&this->list, de_NOAPI_entity_update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(&this->list, de_NOAPI_entity_destroy, 1);
    uplist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    uplist_iterator(&this->list, de_NOAPI_entity_destroy, 1);
    uplist_end(&this->list);
}