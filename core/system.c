#include "system.h"

#include "../config/va_arg.h"

#include "../libs/uplist.h"

void de_system_init(de_system *const this, de_system_f const updateFn, unsigned int maxItems, unsigned int params)
{
    this->updateFn = updateFn;
    this->params = params;
    this->errorHandler = 0;

    uplist_init(&this->upl, maxItems * this->params);
}

void de_system_add(de_system *const this, ...)
{
    unsigned int params = this->params;
    void (*eh)(de_system *const) = this->errorHandler;
    uplist *const upl = &this->upl;

    va_list ap;
    va_start(ap, this);

    if (eh)
    {
        while (params--)
            if (uplist_add(upl, va_arg(ap, void *const)) < 0)
                eh(this);
    }
    else
        while (params--)
            uplist_add(upl, va_arg(ap, void *const));

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