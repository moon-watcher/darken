#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this)
{
    uclist_init(this, 0);
}

void *de_system_add(de_system *const this, void *const data)
{
    void *ret = uclist_add(this, data);

    if (ret == UCLIST_ALLOC_ERROR)
    {
        DARKEN_LOG("de_system_add: no allocated");
    }

    return ret;
}

int de_system_delete(de_system *const this, void *const data)
{
    int ret = uclist_remove(this, data, 0);

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

int de_system_update(de_system *const this, void (*update)(), unsigned params)
{
    int ret = uclist_iterator(this, update, params);

#if DARKEN_LOG
    switch (ret)
    {
    case UCLIST_NO_NBITEMS:
        DARKEN_LOG("de_system_update: params is 0");
        break;
    case UCLIST_NO_ITERATOR:
        DARKEN_LOG("de_system_update: no iterator");
        break;
    case UCLIST_NO_COUNT:
        DARKEN_LOG("de_system_update: count is 0");
        break;
    }
#endif

    return ret;
}

void de_system_reset(de_system *const this)
{
    uclist_reset(this);
}

void de_system_end(de_system *const this)
{
    uclist_end(this);
}
