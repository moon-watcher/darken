#include "system.h"

void de_system_init(de_system_t *$, void (*update)(), uint16_t params)
{
    $->update_f = update;
    $->params = params;

    uclist_init(&$->list, 0);
}

uint16_t de_system_add(de_system_t *$, void *data[])
{
    uint16_t i = $->params;

    while (i--)
        if (uclist_addSafe(&$->list, *data))
            data++;
        else
            return i + 1;

    return 0;
}

inline void de_system_addUnsafe(de_system_t *$, void *data[])
{
    uint16_t i = $->params;

    while (i--)
        uclist_add(&$->list, *data++);
}

uint16_t de_system_delete(de_system_t *$, void *data)
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

void de_system_update(de_system_t *$)
{
    if ($->list.size)
        $->update_f(&$->list, $->params);
}

void de_system_reset(de_system_t *$)
{
    uclist_reset(&$->list);
}

void de_system_end(de_system_t *$)
{
    uclist_end(&$->list);
}


// void de_system_updateLoopExpand(de_system_t *$)
// {
//     int16_t size = $->list.size;
//     if (!size) return;

//     uint16_t params = $->params;
//     void **items = (void **)$->list.items;
//     uint16_t i = size / params;
//     void (*update_f)() = $->update_f;

//     while (i--)
//     {
//         update_f(items[0], items[1], items[2], items[3]);
//         items += params;
//     }
// }

// void de_system_updateLoop(de_system_t *$)
// {
//     int16_t size = $->list.size;
//     if (!size) return;
    
//     register uint16_t params = $->params;
//     void **items = (void **)$->list.items;
//     void **const end = items + size;
//     void (*update_f)(void **) = $->update_f;
    
//     while (items < end)
//     {
//         update_f(items);
//         items += params;
//     }    
// }
