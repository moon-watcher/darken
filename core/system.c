#include "system.h"

#include "../config/va_arg.h"

#include "../libs/culist.h"

void de_system_init(de_system *const this, de_system_f const updateFn, unsigned int maxItems, unsigned int params)
{
    this->updateFn = updateFn;
    this->maxItems = maxItems;
    this->params = params;
    this->errorHandler = 0;

    culist_init(&this->list, this->maxItems * this->params, 0);
}

void de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    unsigned int params = this->params;
    void (*eh)(de_system *const) = this->errorHandler;
    culist *const cu = &this->list;

    while (params--)
        if (culist_set(cu, va_arg(ap, void *const)) < 0 && eh)
            eh(this);

    va_end(ap);
}

void de_system_delete(de_system *const this, void *const data)
{
    uplist_removeByData(&this->list.upl, data, this->params);
}

void de_system_update(de_system *const this)
{
    uplist_iterator(&this->list.upl, this->updateFn, this->params);
}

void de_system_end(de_system *const this)
{
    culist_end(&this->list, 0);
}