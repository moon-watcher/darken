#include "manager.h"

static void nullf() { }

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    this->update = &de_entity_update;
    this->destroy = &de_entity_destroy;
}

de_entity *de_manager_new(de_manager *const this)
{
    de_entity *entity = uplist_alloc(&this->list);

    entity->update = nullf;
    entity->state = &(de_state){nullf, nullf, nullf};
    entity->xtor = &(de_state){nullf, nullf, nullf};

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(&this->list, this->update, 1);
}

unsigned de_manager_delete(de_manager *const this, void *const item)
{
    int index = uplist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uplist_remove(&this->list, item);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(&this->list, this->destroy, 1);
    uplist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(&this->list);
}