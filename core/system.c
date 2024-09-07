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
        _DARKEN_DEBUG_NOTICE("system datasize is 0");
    }
    else if ((this->data = malloc(datasize)) == 0)
    {
        _DARKEN_DEBUG_ERROR("system malloc() is null");
    }
    else
    {
        _DARKEN_DEBUG_INFO("system init");
    }
}

void de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
    {
        if (uclist_add(list, va_arg(ap, void *const)) == 0)
        {
            _DARKEN_DEBUG_ERROR("system add reference");
        }
        else
        {
            _DARKEN_DEBUG_INFO("system added");
        }
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
        int ret = uclist_remove(list, va_arg(ap, void *const), 0);

#if _DARKEN_DEBUG
        switch (ret)
        {
        case -1:
            _DARKEN_DEBUG_WARNING("system, ref not found");
            break;
        case -2:
            _DARKEN_DEBUG_WARNING("system, this->count");
            break;
        default:
            _DARKEN_DEBUG_INFO("system _destoy");
            break;
        }
#endif
    }

    va_end(ap);
}

int de_system_update(de_system *const this)
{
    int ret = uclist_iterator(&this->list, this->update, this->params);

#if _DARKEN_DEBUG
    switch (ret)
    {
    case 1:
        _DARKEN_DEBUG_INFO("system, update");
        break;
    case -4:
        _DARKEN_DEBUG_WARNING("system, this->params");
        break;
    case -3:
        _DARKEN_DEBUG_WARNING("system, this->update");
        break;
    case -2:
        _DARKEN_DEBUG_WARNING("system, this->count");
        break;
    default:
        _DARKEN_DEBUG_ERROR("system, WTF?! #2");
        break;
    }
#endif

    return ret;
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
    {
        free(this->data);
    }

    memset(this, 0, sizeof(de_system));

    _DARKEN_DEBUG_INFO("system end");
}