#include "system.h"
#include "../libs/uplist.h" 
#include "../services/va_arg.h"

void de_system_init(de_system *const this, de_system_f updateFn, unsigned params)
{
    this->updateFn = updateFn;
    this->params = params ?: 1;
    this->errorHandler = 0;

    uplist_init(&this->upl, 0);
}

void de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    unsigned params = this->params;
    
    if (this->errorHandler == 0)
        while (params--)
            uplist_add(&this->upl, va_arg(ap, void *const));
    else
        while (params--)
            if (uplist_add(&this->upl, va_arg(ap, void *const)) == 0)
                this->errorHandler(this);
}

void de_system_delete(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    unsigned params = this->params;

    while (params--)
        uplist_remove(&this->upl, va_arg(ap, void *const));
}

void de_system_update(de_system *const this)
{
    uplist_iterator(&this->upl, this->updateFn, this->params);
}

void de_system_end(de_system *const this)
{
    uplist_end(&this->upl);
}