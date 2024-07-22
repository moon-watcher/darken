#include "vanillamanager.h"

void vanillamanager_init(vanillamanager *const this, unsigned bytes, void(*update), void(*destroy))
{
    uplist_initAlloc(&this->list, bytes);
    this->update = update;
    this->destroy = destroy;
}

void *vanillamanager_new(vanillamanager *const this)
{
    return uplist_alloc(&this->list);
}

void vanillamanager_update(vanillamanager *const this)
{
    uplist_iterator(&this->list, this->update, 1);
}

unsigned vanillamanager_delete(vanillamanager *const this, void *const item)
{
    int index = uplist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uplist_remove(&this->list, item, 0);
}

void vanillamanager_reset(vanillamanager *const this)
{
    uplist_iterator(&this->list, this->destroy, 1);
    uplist_reset(&this->list);
}

void vanillamanager_end(vanillamanager *const this)
{
    vanillamanager_reset(this);
    uplist_end(&this->list);
}