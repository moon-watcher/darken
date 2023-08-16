#include "system.h"

#include "../config/va_arg.h"

#include "../libs/uplist.h"

void de_system_init(de_system *const this, de_system_f const updateFn, unsigned int maxItems, unsigned int params)
{
    this->updateFn = updateFn;
    this->maxItems = maxItems;
    this->params = params;
    this->errorHandler = 0;

    uplist_init(&this->upl, this->maxItems * this->params);
}

void de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    for (unsigned int i = 0; i < this->params; i++)
        if (uplist_add(&this->upl, va_arg(ap, void *const)) < 0 && this->errorHandler)
            this->errorHandler(this);

    va_end(ap);
}

void de_system_delete(de_system *const this, void *const data)
{
    uplist_removeByData(&this->upl, data, this->params);
}

void de_system_update(de_system *const this)
{
    uplist_iterator(&this->upl, this->updateFn, this->params);
}

void de_system_end(de_system *const this)
{
    uplist_end(&this->upl);
}

void de_system_errorHandler(de_system *const this, void (*eh)(de_system *const))
{
    this->errorHandler = eh;
}
