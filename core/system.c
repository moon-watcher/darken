#include "system.h"

#include "../config/va_arg.h"

#include "../libs/upl.h"

void deSystem_init(deSystem_t *const this, deSystem_f const updateFn, unsigned int maxItems, unsigned int params)
{
    this->updateFn = updateFn;
    this->maxItems = maxItems;
    this->params = params;
    this->errorHandler = 0;

    upl_init(&this->upl, this->maxItems * this->params);
}

void deSystem_add(deSystem_t *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    for (unsigned int i = 0; i < this->params; i++)
        if (upl_add(&this->upl, va_arg(ap, void *const)) < 0 && this->errorHandler)
            this->errorHandler(this);

    va_end(ap);
}

void deSystem_delete(deSystem_t *const this, void *const data)
{
    upl_removeByData(&this->upl, data, this->params);
}

void deSystem_update(deSystem_t *const this)
{
    upl_iterator(&this->upl, this->updateFn, this->params);
}

void deSystem_end(deSystem_t *const this)
{
    upl_end(&this->upl);
}

void deSystem_errorHandler(deSystem_t *const this, void (*eh)(deSystem_t *const))
{
    this->errorHandler = eh;
}
