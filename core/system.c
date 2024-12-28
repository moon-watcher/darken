#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this)
{
    uclist_init(this, 0);
}

void *de_system_add(de_system *const this, void *const data)
{
    void *const ret = uclist_add(this, data);

    if (ret == 0)
    {
        DARKEN_LOG("de_system_add(): allocation");
    }

    return ret;
}

int de_system_delete(de_system *const this, void *const data)
{
    int ret = uclist_remove(this, data, 0);

    if (ret == UCLIST_NOT_FOUND)
    {
        DARKEN_LOG("de_system_delete(): not found");
    }

    return ret;
}

int de_system_update(de_system *const this, void (*update)(), unsigned params)
{
    if (this->size == 0)
    {
        return 0;
    }
    else if (update == 0)
    {
        DARKEN_LOG("de_system_update(): null iterator");
        return UCLIST_NO_ITERATOR;
    }
    else if (params == 0)
    {
        DARKEN_LOG("de_system_update(): params is 0");
        return UCLIST_NO_NBITEMS;
    }

    for (unsigned i = 0; i < this->size; i += params)
    {
        update(this->list[i + 0], this->list[i + 1], this->list[i + 2], this->list[i + 3]);
    }

    return this->size / params;
}

void de_system_reset(de_system *const this)
{
    uclist_reset(this);
}

void de_system_end(de_system *const this)
{
    uclist_end(this);
}
