#include "system.h"
#include "../config.h"

#define _DARKEN_LOG(EXPRESION, MSG)        \
    if (EXPRESION)                         \
    {                                      \
        DARKEN_LOG("de_system_add: " MSG); \
    }

//

void de_system_init(de_system *const this)
{
    uclist_init(this, 0);
}

void *de_system_add(de_system *const this, void *const data)
{
    void *ret = uclist_add(this, data);

    _DARKEN_LOG(ret == 0, "allocation");

    return ret;
}

int de_system_delete(de_system *const this, void *const data)
{
    int ret = uclist_remove(this, data, 0);

    _DARKEN_LOG(ret == UCLIST_NOT_FOUND, "reference not found");

    return ret;
}

int de_system_update(de_system *const this, void (*update)(), unsigned params)
{
    int ret = uclist_iterator(this, update, params);

    _DARKEN_LOG(ret == UCLIST_NO_NBITEMS, "params is 0");
    _DARKEN_LOG(ret == UCLIST_NO_ITERATOR, "no iterator");

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
