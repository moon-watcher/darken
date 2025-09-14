#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update_f = update;
    this->params = params;

    uclist_init(&this->list, 0);
}

unsigned de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    uclist *const list = &this->list;
    unsigned params = this->params;

    while (params)
    {
        void *const data = va_arg(ap, void *const);

        if (uclist_find(list, data) < 0 && 0 == uclist_add(list, data))
            return params;

        --params;
    }

    return params;
}

int de_system_delete(de_system *const this, void *const data)
{
    uclist *const list = &this->list;
    int index = uclist_find(list, data);

    if (index < 0)
        return index;

    unsigned params = this->params;
    void **const items = de_system_getItems(list);
    void **src = &items[list->size -= params];
    void **dst = &items[index];

    while (params--)
    {
        void *temp = *dst;
        *dst++ = *src;
        *src++ = temp;
    }

    return index;
}

void de_system_update(de_system *const this)
{
    uclist *const list = &this->list;
    unsigned params = this->params;
    void **items = de_system_getItems(list);
    unsigned size = de_system_getSize(list) / params;
    void (*update_f)() = this->update_f;

    while (size--)
    {
        update_f(items[0], items[1], items[2], items[3]);
        items += params;
    }
}

void de_system_reset(de_system *const this)
{
    uclist_reset(&this->list);
}

void de_system_end(de_system *const this)
{
    uclist_end(&this->list);
}

//

inline unsigned de_system_getSize(de_system *const this)
{
    return this->list.size;
}

inline unsigned de_system_getCapacity(de_system *const this)
{
    return this->list.capacity;
}

inline void **de_system_getItems(de_system *const this)
{
    return (void **)this->list.items;
}
