#include "manager.h"
#include "entity.h"
#include "NOAPI.h"
#include "../config.h"

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
        NOAPI_entity_array[entity->ctrl](entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uplist *const list = &this->list;
    unsigned const count = list->count;

    for (unsigned i = 0; i < count; i++)
        NOAPI_entity_destroy(list->items[i]);

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