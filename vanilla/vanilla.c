#include "vanilla.h"

void vanilla_init(vanilla *const this, unsigned bytes, void(*update), void(*destroy))
{
    uplist_initAlloc(&this->list, bytes);
    this->update = update;
    this->destroy = destroy;
}

void *vanilla_new(vanilla *const this)
{
    return uplist_alloc(&this->list);
}

void vanilla_update(vanilla *const this)
{
    uplist_iterator(&this->list, this->update, 1);
}

unsigned vanilla_delete(vanilla *const this, void *const item)
{
    int index = uplist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uplist_remove(&this->list, item);
}

void vanilla_reset(vanilla *const this)
{
    uplist_iterator(&this->list, this->destroy, 1);
    uplist_reset(&this->list);
}

void vanilla_end(vanilla *const this)
{
    vanilla_reset(this);
    uplist_end(&this->list);
}