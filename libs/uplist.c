// Unordered list of pointers

#include "uplist.h"

#include "../config/free.h"
#include "../config/malloc.h"
#include "../config/realloc.h"

#define IT(F, ...)                                                                         \
    static void F(void **const list, unsigned int *const n, void (*it)(), unsigned int nb) \
    {                                                                                      \
        for (unsigned int i = 0; i < *n; i += nb)                                          \
            it(__VA_ARGS__);                                                               \
    }

IT(f0, );
IT(f1, list[i + 0]);
IT(f2, list[i + 0], list[i + 1]);
IT(f3, list[i + 0], list[i + 1], list[i + 2]);
IT(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
IT(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
IT(f6, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
IT(f7, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);

static void (*const it_funcs[])() = { f0, f1, f2, f3, f4, f5, f6, f7, };


void uplist_init(uplist *const this, unsigned int size)
{
    this->size = size ?: 1;
    this->list = malloc(this->size * sizeof(void *));
    this->next = 0;
    this->resizable = 0;
}

int uplist_add(uplist *const this, void *const add)
{
    unsigned int const next = this->next;

    if (next >= this->size)
    {
        if (this->resizable == 0)
            return -2;

        else if (uplist_resize(this, this->size + this->size / 2) == 0)
            return -1;
    }

    this->list[next] = add;
    ++this->next;

    return next;
}

void uplist_iterator(uplist *const this, void (*iterator)(), unsigned int nbItems)
{
    if (iterator != 0)
        it_funcs[nbItems](this->list, &this->next, iterator, nbItems);
}

void uplist_remove(uplist *const this, unsigned int index)
{
    unsigned int *const next = &this->next;

    if (*next == 0)
        return;

    void **const list = this->list;
    list[index] = list[--*next];
}

void uplist_end(uplist *const this)
{
    free(this->list);
}

void uplist_reset(uplist *const this)
{
    this->next = 0;
}

void uplist_removeByData(uplist *const this, void *const data, unsigned int nbItems)
{
    int const index = uplist_find(this, data);

    if (index < 0)
        return;

    void **const list = this->list;
    unsigned int *const next = &this->next;

    for (unsigned int j = 0; j < nbItems; j++)
        list[index + j] = list[*next - (nbItems - j)];

    *next -= nbItems;
}

unsigned int uplist_resize(uplist *const this, unsigned int size)
{
    if (this->size == size)
        return 2;

    this->size = size;
    this->list = realloc(this->list, this->size * sizeof(void *));

    return this->list != 0;
}

int uplist_find(uplist *const this, void *const data)
{
    void **const list = this->list;
    unsigned int *const next = &this->next;

    for (unsigned int i = 0; i < *next; i++)
        if (list[i] == data)
            return i;

    return -1;
}

// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned int nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     void **const list = this->list;
//     unsigned int *const next = &this->next;

//     for (unsigned int i = 0; i < *next; i += nbItems)
//         iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
// }

// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned int nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     unsigned int const items = nbItems;

//     void **const list = this->list;
//     unsigned int *const next = &this->next;

//     if (items == 1)
//         for (unsigned int i = 0; i < *next; i += items)
//             iterator(list[i + 0]);

//     else if (items == 2)
//         for (unsigned int i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1]);

//     else if (items == 3)
//         for (unsigned int i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2]);

//     else if (items == 4)
//         for (unsigned int i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);

//     else if (items == 5)
//         for (unsigned int i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

//     else
//         for (unsigned int i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
// }