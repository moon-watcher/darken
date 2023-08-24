// Unordered list of pointers

#include "uplist.h"

#include "../config/free.h"
#include "../config/malloc.h"
#include "../config/realloc.h"

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
    unsigned int const items = nbItems;

    if (iterator == 0 || items == 0)
        return;

    void **const list = this->list;
    unsigned int *const next = &this->next;

    if (items == 1)
        for (unsigned int i = 0; i < *next; i += items)
            iterator(list[i + 0]);

    else if (items == 2)
        for (unsigned int i = 0; i < *next; i += items)
            iterator(list[i + 0], list[i + 1]);

    else if (items == 3)
        for (unsigned int i = 0; i < *next; i += items)
            iterator(list[i + 0], list[i + 1], list[i + 2]);

    else if (items == 4)
        for (unsigned int i = 0; i < *next; i += items)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);

    else if (items == 5)
        for (unsigned int i = 0; i < *next; i += items)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

    else
        for (unsigned int i = 0; i < *next; i += items)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
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

//     void f1(void **const list, unsigned int *const next, void (*iterator)(), unsigned int nbItems)
//     {
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0]);
//     }
//     void f2(void **const list, unsigned int *const next, void (*iterator)(), unsigned int nbItems)
//     {
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1]);
//     }
//     void f3(void **const list, unsigned int *const next, void (*iterator)(), unsigned int nbItems)
//     {
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2]);
//     }
//     void f4(void **const list, unsigned int *const next, void (*iterator)(), unsigned int nbItems)
//     {
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
//     }
//     void f5(void **const list, unsigned int *const next, void (*iterator)(), unsigned int nbItems)
//     {
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
//     }
//     void f6(void **const list, unsigned int *const next, void (*iterator)(), unsigned int nbItems)
//     {
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
//     }
//     void f7(void **const list, unsigned int *const next, void (*iterator)(), unsigned int nbItems)
//     {
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);
//     }

//     void **const list = this->list;
//     unsigned int *const next = &this->next;

//     void (*const funcs[])(void **const, unsigned int *const, void (*)(), unsigned int) = { 0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, };
//     funcs[nbItems](this->list, next, iterator, nbItems);
// }

// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned int nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     void **const list = this->list;
//     unsigned int *const next = &this->next;

//     switch (nbItems)
//     {
//     case 1:
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0]);
//         break;
//     case 2:
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1]);
//         break;
//     case 3:
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2]);
//         break;
//     case 4:
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
//         break;
//     case 5:
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
//         break;
//     case 6:
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
//         break;
//     case 7:
//         for (unsigned int i = 0; i < *next; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);
//         break;
//     }
// }

// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned int nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     void **const list = this->list;
//     unsigned int *const next = &this->next;

//     for (unsigned int i = 0; i < *next; i += nbItems)
//         iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
// }