#include "system.h"
#include "../config.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update_f = update;
    this->params = params;
    this->pause = 0;

    uclist_init(&this->list, 0);
}

unsigned de_system_add(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    uclist *const list = &this->list;
    unsigned count = this->params;

    while (count--)
        if (0 == uclist_add(list, va_arg(ap, void *const)))
            return 0;

    return 1;
}

int de_system_delete(de_system *const this, void *const data)
{
    uclist *const list = &this->list;
    int index = uclist_find(list, data);

    if (index < 0)
        return -1;

    unsigned count = this->params;
    void **const items = (void **)list->items;
    void **src = &items[list->size -= count];
    void **dst = &items[index];

    while (count--)
    {
        void *temp = *dst;
        *dst++ = *src;
        *src++ = temp;
    }

    return index;
}

#define ITERATOR_CASE(N, ...)      \
    case N:                        \
        while (size--)             \
        {                          \
            update_f(__VA_ARGS__); \
            items += N;            \
        }                          \
        return;

void de_system_update(de_system *const this)
{
    if (this->pause)
        return;

    uclist *const list = &this->list;
    unsigned count = this->params;
    void **items = list->items;
    unsigned size = list->size / count;
    void (*update_f)() = this->update_f;

    switch (count)
    {
        ITERATOR_CASE(1, items[0])
        ITERATOR_CASE(2, items[0], items[1])
        ITERATOR_CASE(3, items[0], items[1], items[2])
        ITERATOR_CASE(4, items[0], items[1], items[2], items[3])
        ITERATOR_CASE(5, items[0], items[1], items[2], items[3], items[4])
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

// void de_system_update(de_system *const this)
// {
//     if (this->pause)
//         return;

//     uclist *const list = &this->list;
//     unsigned count = this->params;
//     void **items = list->items;
//     unsigned size = list->size / count;

//     while (size--)
//     {
//         this->update_f(items[0], items[1], items[2], items[3]);
//         items += count;
//     }
// }