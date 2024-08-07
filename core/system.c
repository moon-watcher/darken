#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params, unsigned datasize)
{
    this->update = update;
    this->params = params ?: 1;

    uclist_init(&this->list);

    this->data = 0;

    if (datasize == 0)
    {
        // LOG("NOTICE: datasize is 0");
    }
    else if ((this->data = malloc(datasize)) == 0)
    {
        LOG("ERROR: malloc() returns null");
    }
}

void de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
        if (uclist_add(list, va_arg(ap, void *const)) == 0)
            LOG("ERROR: Add reference to system");

    va_end(ap);
}

void de_system_delete(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
        if (uclist_remove(list, va_arg(ap, void *const), 0) == 0)
            LOG("ERROR: Remove reference from system");

    va_end(ap);
}

void de_system_update(de_system *const this)
{
    uclist_iterator(&this->list, this->update, this->params);
}

void de_system_reset(de_system *const this)
{
    uclist_reset(&this->list);
}

unsigned de_system_count(de_system *const this)
{
    return this->list.count;
}

unsigned de_system_capacity(de_system *const this)
{
    return this->list.capacity;
}

void de_system_end(de_system *const this)
{
    uclist_end(&this->list);

    if (this->data != 0)
        free(this->data);

    memset(this, 0, sizeof(de_system));
}