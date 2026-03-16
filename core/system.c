#include "system.h"

void de_system_init(de_system *$, void *(*update)(), uint16_t params)
{
    $->update = update;
    $->params = params;

    uclist_init(&$->list, 0);
}

uint16_t de_system_add(de_system *$, void *data[])
{
    uint16_t i = $->params;

    while (i--)
        if (!uclist_add(&$->list, *data++))
            return i + 1;

    return 0;
}

uint16_t de_system_addFast(de_system *$, void *data[])
{
    uint16_t i = $->params;

    while (i--)
        if (!uclist_addUnsafe(&$->list, *data++))
            return i + 1;

    return 0;
}

uint16_t de_system_delete(de_system *$, void *data)
{
    int16_t index = uclist_getIndex(&$->list, data);

    if (index < 0)
        return 0;

    uint16_t i = $->params;
    void **items = (void **)$->list.items;
    void **src = &items[$->list.size -= i];
    void **dst = &items[index];

    while (i--)
    {
        void *temp = *dst;
        *dst++ = *src;
        *src++ = temp;
    }

    return 1;
}

void de_system_update(de_system *$)
{
    if ($->list.size && $->update)
        $->update($);
}

void de_system_reset(de_system *$)
{
    uclist_reset(&$->list);
}

void de_system_end(de_system *$)
{
    uclist_end(&$->list);
}
