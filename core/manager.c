#include "manager.h"
#include "entity.h"

#include "../libs/uplist.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(&this->list, bytes);
    this->destroy = de_entity_destroy;
    this->update = de_entity_update;
}

void *de_manager_new(de_manager *const this)
{
    return uplist_alloc(&this->list);
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
    uplist_iterator(&this->list, this->destroy, 1);
    uplist_reset(&this->list);
    uplist_end(&this->list);
}