#include "system.h"
#include "../debug.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params, unsigned datasize)
{
    this->update = update;
    this->params = params ?: 1;

    uclist_init(&this->list);

#if DARKEN_DEBUG
    if (datasize == 0)
    {
        DARKEN_NOTICE("system datasize is 0");
    }
    else if ((this->data = malloc(datasize)) == 0)
    {
        DARKEN_ERROR("system malloc() is null");
    }
#else
    datasize && (this->data = malloc(datasize));
#endif
}

void de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
    {
#if DARKEN_DEBUG
        if (uclist_add(list, va_arg(ap, void *const)) == 0)
        {
            DARKEN_ERROR("system add reference");
        }
#else
        uclist_add(list, va_arg(ap, void *const));
#endif
    }

    va_end(ap);
}

void de_system_delete(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
    {
#if DARKEN_DEBUG
        switch (uclist_remove(list, va_arg(ap, void *const), 0))
        {
        case -1:
            DARKEN_WARNING("system, ref not found");
            break;
        case -2:
            DARKEN_WARNING("system, this->count");
        }
#else
        uclist_remove(list, va_arg(ap, void *const), 0);
#endif
    }

    va_end(ap);
}

int de_system_update(de_system *const this)
{
    int ret = uclist_iterator(&this->list, this->update, this->params);

#if DARKEN_DEBUG
    switch (ret)
    {
    case 1:
        break;
    case -4:
        DARKEN_WARNING("system, this->params");
        break;
    case -3:
        DARKEN_WARNING("system, this->update");
        break;
    case -2:
        DARKEN_WARNING("system, this->count");
        break;
    default:
        DARKEN_ERROR("system, WTF?! #2");
    }
#endif

    return ret;
}

void de_system_reset(de_system *const this)
{
    uclist_reset(&this->list);
}

void de_system_end(de_system *const this)
{
    uclist_end(&this->list);
    MEM_free(this->data);
    memset(this, 0, sizeof(de_system));
}
