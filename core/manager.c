#include "manager.h"
#include "entity.h"
#include "../config.h"
#include "../NOAPI/state.h"
#include "../NOAPI/entity.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(&this->list, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, void (*desctructor)())
{
    de_entity *entity = uplist_alloc(&this->list);

    if (entity != 0)
    {
        entity->update = de_NOAPI_state_nullf;
        entity->leave = de_NOAPI_state_nullf;
        entity->destructor = desctructor ?: de_NOAPI_state_nullf;
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
        de_NOAPI_entity_array[entity->ctrl](entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uplist *const list = &this->list;
    unsigned const count = list->count;

    for (unsigned i = 0; i < count; i++)
        de_NOAPI_entity_destroy(list->items[i]);

    uplist_reset(list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(&this->list);
}