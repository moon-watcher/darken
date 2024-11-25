#include "manager.h"
#include "../debug.h"

void de_manager_initialize(de_manager *const this, unsigned size, void(*update), void(*destroy))
{
    uclist_init(&this->list, size);
    this->update = update;
    this->destroy = destroy;
}

void de_manager_update(de_manager *const this)
{
    uclist_iterator(&this->list, this->update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(&this->list, this->destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    uclist_iterator(&this->list, this->destroy, 1);
    uclist_reset(&this->list);
    uclist_end(&this->list);
}
