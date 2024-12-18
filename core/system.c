#include "system.h"
#include "../config.h"

#define _PROCESS(EXP, OK, ERR, MSG)    \
    if (EXP)                           \
    {                                  \
        DARKEN_LOG("de_system: " MSG); \
        ERR;                           \
    }                                  \
    OK

//

void de_system_init(de_system *const this)
{
    uclist_init(this, 0);
}

void *de_system_add(de_system *const this, void *const data)
{
    void *ret = uclist_add(this, data);

    _PROCESS(ret == 0, return ret, , "allocation");
}

int de_system_delete(de_system *const this, void *const data)
{
    int ret = uclist_remove(this, data, 0);

    _PROCESS(ret == UCLIST_NOT_FOUND, return ret, , "reference not found");
}

void de_system_update(de_system *const this, void (*update)(), unsigned params)
{
    _PROCESS(update == 0, , return UCLIST_NO_ITERATOR, "no iterator");
    _PROCESS(params == 0, , return UCLIST_NO_NBITEMS, "params is 0");

    unsigned count = this->count;

    for (unsigned i = 0; i < count; i += params)
    {
        update(this->list[i + 0], this->list[i + 1], this->list[i + 2], this->list[i + 3]);
    }
}

void de_system_reset(de_system *const this)
{
    uclist_reset(this);
}

void de_system_end(de_system *const this)
{
    uclist_end(this);
}
