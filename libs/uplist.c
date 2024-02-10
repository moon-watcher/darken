// Unordered list of pointers

#include "uplist.h"

#include "../services/free.h"
#include "../services/malloc.h"

void uplist_init(uplist *const this, unsigned size)
{
    this->size = size ?: 1;
    this->list = malloc(this->size * sizeof(void *));
    this->next = 0;
    this->resizeBy = this->size;
}

int uplist_add(uplist *const this, void *const add)
{
    unsigned const next = this->next;

    if (next >= this->size && uplist_resize(this, 0) == 0)
        return -1;

    this->list[next] = add;
    ++this->next;

    return next;
}

int uplist_resize(uplist *const this, unsigned increment)
{
    unsigned const oldSize = this->size * sizeof(void *);
    this->size += increment ?: this->resizeBy;

    void *p = malloc(this->size * sizeof(void *));

    if (p == 0)
        return 0;

    memcpy(p, this->list, oldSize);
    free(this->list);

    this->list = p;

    return 1;
}

void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
{
    if (iterator == 0)
        return;

    #define IT(F, ...)                                                          \
        void F(void **const list, unsigned *const n, void (*it)(), unsigned nb) \
        {                                                                       \
            for (unsigned i = 0; i < *n; i += nb)                               \
                it(__VA_ARGS__);                                                \
        }

    IT(f0, );
    IT(f1, list[i + 0]);
    IT(f2, list[i + 0], list[i + 1]);
    IT(f3, list[i + 0], list[i + 1], list[i + 2]);
    IT(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
    IT(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
    IT(f6, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
    IT(f7, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);

    void (*const funcs[])() = { f0, f1, f2, f3, f4, f5, f6, f7, };

    funcs[nbItems](this->list, &this->next, iterator, nbItems);
}

void uplist_iteratorEx(uplist *const this, void (*iterator)(), unsigned nbItems)
{
    iterator(this->list, nbItems);
}

unsigned uplist_remove(uplist *const this, unsigned index)
{
    unsigned *const next = &this->next;

    if (*next == 0)
        return 0;

    void **const list = this->list;
    list[index] = list[--*next];

    return 1;
}

void uplist_end(uplist *const this)
{
    free(this->list);
}

void uplist_reset(uplist *const this)
{
    this->next = 0;
}

unsigned uplist_removeByData(uplist *const this, void *const data, unsigned nbItems)
{
    int const index = uplist_find(this, data);

    if (index < 0)
        return 0;

    void **const list = this->list;
    unsigned *const next = &this->next;

    for (unsigned j = 0; j < nbItems; j++)
        list[index + j] = list[*next - (nbItems - j)];

    *next -= nbItems;

    return 1;
}

int uplist_find(uplist *const this, void *const data)
{
    void **const list = this->list;
    unsigned *const next = &this->next;

    for (unsigned i = 0; i < *next; i++)
        if (list[i] == data)
            return i;

    return -1;
}

// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     void **const list = this->list;
//     unsigned *const next = &this->next;

//     for (unsigned i = 0; i < *next; i += nbItems)
//         iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
// }

// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     unsigned const items = nbItems;

//     void **const list = this->list;
//     unsigned *const next = &this->next;

//     if (items == 1)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0]);

//     else if (items == 2)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1]);

//     else if (items == 3)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2]);

//     else if (items == 4)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);

//     else if (items == 5)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

//     else
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
// }