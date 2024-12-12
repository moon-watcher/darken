#include "system.h"
#include "../config.h"

unsigned de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    if (update == 0)
    {
        DARKEN_LOG("de_system_init: update is null");
        return 0;
    }

    if (params == 0)
    {
        DARKEN_LOG("de_system_init: params forced to 1");
        params = 1;
    }

    this->update = update;
    this->params = params;

    uclist_init(&this->list, 0);

    return 1;
}

void *de_system_add(de_system *const this, void *const data)
{
    void *ret = uclist_add(&this->list, data);

    if (ret == UCLIST_ALLOC_ERROR)
    {
        DARKEN_LOG("de_system_add: no allocated");
    }

    return ret;
}

int de_system_delete(de_system *const this, void *const data)
{
    int ret = uclist_remove(&this->list, data, 0);

#if DARKEN_LOG
    switch (ret)
    {
    case UCLIST_NOT_FOUND:
        DARKEN_LOG("de_system_delete: reference not found");
        break;
    case UCLIST_NO_COUNT:
        DARKEN_LOG("de_system_delete: count is 0");
        break;
    }
#endif

    return ret;
}

void de_system_update(de_system *const this)
{
    uclist_iterator(&this->list, this->update, this->params);
}

void de_system_reset(de_system *const this)
{
    uclist_reset(&this->list);
}

void de_system_end(de_system *const this)
{
    uclist_end(&this->list);
}

// void de_system_add(de_system *const this, ...)
// {
//     va_list ap; va_start(ap, this);
//     for (unsigned i = 0; i < this->params; i++)
//         uclist_add(&this->list, va_arg(ap, void *const)));
// }

// void de_system_delete(de_system *const this, ...)
// {
//     va_list ap; va_start(ap, this);
//     for (unsigned i = 0; i < this->params; i++)
//         uclist_remove(&this->list, va_arg(ap, void *const), 0);
// }
