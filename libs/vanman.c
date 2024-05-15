#include "vanman.h"

void vanman_init(vanman *const this, unsigned bytes, void(*update), void(*destroy))
{
    uplist_initAlloc(&this->list, bytes);
    this->update = update;
    this->destroy = destroy;
}

void *vanman_new(vanman *const this)
{
    return uplist_alloc(&this->list);
}

void vanman_update(vanman *const this)
{
    uplist_iterator(&this->list, this->update, 1);
}

unsigned vanman_delete(vanman *const this, void *const item)
{
    int index = uplist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uplist_remove(&this->list, item);
}

void vanman_reset(vanman *const this)
{
    uplist_iterator(&this->list, this->destroy, 1);
    uplist_reset(&this->list);
}

void vanman_end(vanman *const this)
{
    vanman_reset(this);
    uplist_end(&this->list);
}