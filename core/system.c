#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update = update;
    this->params = params ?: 1;

    uplist_init(&this->list);
}

void de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uplist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
        uplist_add(list, va_arg(ap, void *const));

    va_end(ap);
}

void de_system_delete(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uplist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
        uplist_remove(list, va_arg(ap, void *const), 0);
        
    va_end(ap);
}

void de_system_update(de_system *const this)
{
    uplist_iterator(&this->list, this->update, this->params);
}

void de_system_end(de_system *const this)
{
    uplist_end(&this->list);
}