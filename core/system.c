#include "system.h"
#include "../libs/uplist.h"
#include "../config/va_arg.h"

void de_system_init(de_system *const this, de_system_f const updateFn, unsigned params)
{
    this->updateFn = updateFn;
    this->params = params ?: 1;
    this->errorHandler = 0;

    uplist_init(&this->upl, this->params);
}

void de_system_add(de_system *const this, ...)
{
    unsigned params = this->params;
    uplist *const upl = &this->upl;

    va_list ap;
    va_start(ap, this);

    if (this->errorHandler == 0)
        while (params--)
            uplist_add(upl, va_arg(ap, void *const));
    else
        while (params--)
            if (uplist_add(upl, va_arg(ap, void *const)) == -1)
                this->errorHandler(this);

    va_end(ap);
}

unsigned de_system_delete(de_system *const this, void *const data)
{
    return uplist_removeByData(&this->upl, data, this->params);
}

void de_system_update(de_system *const this)
{
    uplist_iterator(&this->upl, this->updateFn, this->params);
}

void de_system_end(de_system *const this)
{
    uplist_end(&this->upl);
}