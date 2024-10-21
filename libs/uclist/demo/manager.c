#include "manager.h"

void manager_init(manager *const this, unsigned bytes, void(*update), void(*destroy))
{
    uclist_initAlloc(&this->list, bytes);
    this->update = update;
    this->destroy = destroy;
}

void *manager_new(manager *const this)
{
    return uclist_alloc(&this->list);
}

void manager_update(manager *const this)
{
    uclist_iterator(&this->list, this->update, 1);
}

int manager_delete(manager *const this, void *const item)
{
    int index = uclist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uclist_remove(&this->list, item, 0);
}

void manager_reset(manager *const this)
{
    uclist_iterator(&this->list, this->destroy, 1);
    uclist_reset(&this->list);
}

void manager_end(manager *const this)
{
    manager_reset(this);
    uclist_end(&this->list);
}