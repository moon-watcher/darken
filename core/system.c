#include "system.h"
#include "../config.h"

#define _RETURN(RET, EXPRESION, MSG) ({    \
    if (EXPRESION)                         \
    {                                      \
        DARKEN_LOG("de_system_add: " MSG); \
        return RET;                        \
    }                                      \
    RET;                                   \
})

//

void de_system_init(de_system *const this)
{
    uclist_init(this, 0);
}

void *de_system_add(de_system *const this, void *const data)
{
    void *ret = uclist_add(this, data);

    return _RETURN(ret, ret == 0, "allocation");
}

int de_system_delete(de_system *const this, void *const data)
{
    int ret = uclist_remove(this, data, 0);

    return _RETURN(ret, ret == UCLIST_NOT_FOUND, "reference not found");
}

int de_system_update(de_system *const this, void (*update)(), unsigned params)
{
    _RETURN(UCLIST_NO_ITERATOR, update == 0, "no iterator");
    _RETURN(UCLIST_NO_NBITEMS, params == 0, "params is 0");

    unsigned count = this->count;

    for (unsigned i = 0; i < count; i += params)
    {
        update(this->list[i + 0], this->list[i + 1], this->list[i + 2], this->list[i + 3]);
    }

    return count / params;
}

void de_system_reset(de_system *const this)
{
    uclist_reset(this);
}

void de_system_end(de_system *const this)
{
    uclist_end(this);
}
