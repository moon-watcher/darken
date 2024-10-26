#include "system.h"
#include "../debug.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    if (params == 0)
    {
        DARKEN_WARNING("system, set params to 1");
        params = 1;
    }

    this->update = update;
    this->params = params;

    uclist_init(&this->list, 0);
}

int de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;
    
    for (unsigned i = 0; i < params; i++)
    {
        if (0 == uclist_add(list, va_arg(ap, void *const)))
        {
            DARKEN_ERROR("system add reference");
            return 0;
        }
    }

    return 1;
}

int de_system_delete(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);
    unsigned const params = this->params;
    uclist *const list = &this->list;
    int ret = 1;

    for (unsigned i = 0; i < params; i++)
    {
        switch (uclist_remove(list, va_arg(ap, void *const), 0))
        {
#if DARKEN_WARNING
        case -1:
            ret = -1;
            DARKEN_WARNING("system, ref not found");
            break;
        case -2:
            ret = -2;
            DARKEN_WARNING("system, this->count");
            break;
#endif
        }
    }
    
    return ret;
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
