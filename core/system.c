#include "system.h"
#include "../debug.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update = update;
    this->params = params ?: 1;

    uclist_init(&this->list);
}

unsigned de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;
    unsigned i = 0;

    for (; i < params; i++)
    {
        if (0 == uclist_add(list, va_arg(ap, void *const)))
        {
            DARKEN_ERROR("system add reference");
            return 0;
        }
    }

    return i;
}

void de_system_delete(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;

    for (unsigned i = 0; i < params; i++)
    {
        switch (uclist_remove(list, va_arg(ap, void *const), 0))
        {
#if DARKEN_WARNING
        case -1:
            DARKEN_WARNING("system, ref not found");
            break;
        case -2:
            DARKEN_WARNING("system, this->count");
            break;
#endif
        }
    }
}

int de_system_update(de_system *const this)
{
    int ret = uclist_iterator(&this->list, this->update, this->params);

#if DARKEN_WARNING
    switch (ret)
    {
    case -4:
        DARKEN_WARNING("system, this->params");
        break;
    case -3:
        DARKEN_WARNING("system, this->update");
        break;
    case -2:
        DARKEN_WARNING("system, this->count");
        break;
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
}
