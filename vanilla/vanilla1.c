#include "vanilla1.h"

void vanilla1_init(vanilla1 *const this, unsigned bytes, void(*update), void(*destroy))
{
    uplist_initAlloc(&this->list, bytes);
    this->update = update;
    this->destroy = destroy;
}

void *vanilla1_new(vanilla1 *const this)
{
    return uplist_alloc(&this->list);
}

void vanilla1_update(vanilla1 *const this)
{
    uplist_iterator(&this->list, this->update, 1);
}

unsigned vanilla1_delete(vanilla1 *const this, void *const item)
{
    int index = uplist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uplist_remove(&this->list, item);
}

void vanilla1_reset(vanilla1 *const this)
{
    uplist_iterator(&this->list, this->destroy, 1);
    uplist_reset(&this->list);
}

void vanilla1_end(vanilla1 *const this)
{
    vanilla1_reset(this);
    uplist_end(&this->list);
}