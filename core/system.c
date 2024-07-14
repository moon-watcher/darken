#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params, unsigned datasize)
{
    this->update = update;
    this->params = params ?: 1;

    uplist_init(&this->list);

    this->data = 0;

    if (datasize == 0)
    {
        LOG("NOTICE: datasize is 0");
    }
    else if((this->data = malloc(datasize)) == 0)
    {
        LOG("ERROR: malloc() returns null");
    }
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

void de_system_reset(de_system *const this)
{
    uplist_reset(&this->list);
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
    uplist_end(&this->list);

    if (this->data != 0)
        free(this->data);

    memset(this, 0, sizeof(de_system));
}