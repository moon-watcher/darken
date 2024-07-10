#include "system.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update = update;
    this->params = params ?: 1;

    uplist_init(&this->list);
}

void *de_system_add(de_system *const this, void *const data)
{
    return uplist_add(&this->list, data);
}

unsigned de_system_delete(de_system *const this, void *const data)
{
    return uplist_remove(&this->list, data, 0);
}

void de_system_update(de_system *const this)
{
    uplist_iterator(&this->list, this->update, this->params);
}

void de_system_end(de_system *const this)
{
    uplist_end(&this->list);
}