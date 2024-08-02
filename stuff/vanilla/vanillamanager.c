#include "vanillamanager.h"

void vanillamanager_init(vanillamanager *const this, unsigned bytes, void(*update), void(*destroy))
{
    uclist_initAlloc(&this->list, bytes);
    this->update = update;
    this->destroy = destroy;
}

void *vanillamanager_new(vanillamanager *const this)
{
    return uclist_alloc(&this->list);
}

void vanillamanager_update(vanillamanager *const this)
{
    uclist_iterator(&this->list, this->update, 1);
}

unsigned vanillamanager_delete(vanillamanager *const this, void *const item)
{
    int index = uclist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uclist_remove(&this->list, item, 0);
}

void vanillamanager_reset(vanillamanager *const this)
{
    uclist_iterator(&this->list, this->destroy, 1);
    uclist_reset(&this->list);
}

void vanillamanager_end(vanillamanager *const this)
{
    vanillamanager_reset(this);
    uclist_end(&this->list);
}