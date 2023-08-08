#include "upl.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Unordered Pointers List

void upl_init(upl_t *const this, unsigned maxElements)
{
    this->maxElements = maxElements ? maxElements : 1;
    this->list = malloc(this->maxElements * sizeof(void *));
    this->freePos = 0;
}

int upl_add(upl_t *const this, void *const add)
{
    unsigned *const freePos = &this->freePos;

    if (*freePos >= this->maxElements)
    {
        unsigned int max = this->maxElements;

        this->maxElements = max + max / 2;
        void *list = malloc(this->maxElements * sizeof(void *));
        memcpy(list, this->list, max);
        free(this->list);

        this->list = list;
    }

    this->list[*freePos] = add;
    ++*freePos;

    return *freePos - 1;
}

int upl_find(upl_t *const this, void *const data)
{
    void **const list = this->list;
    unsigned *const freePos = &this->freePos;

    for (unsigned i = 0; i < *freePos; i++)
        if (list[i] == data)
            return i;

    return -1;
}

void upl_remove(upl_t *const this, unsigned index)
{
    unsigned *const freePos = &this->freePos;

    if (*freePos == 0)
        return;

    void **const list = this->list;

    --*freePos;
    list[index] = list[*freePos];
}

void upl_removeByData(upl_t *const this, void *const data)
{
    int i = upl_find(this, data);

    if (i < 0)
        return;

    upl_remove(this, i);
}

void upl_bulk_removeByData(upl_t *const this, void *const data, unsigned nbItems)
{
    int i = upl_find(this, data);

    if (i < 0)
        return;

    void **const list = this->list;
    unsigned *const freePos = &this->freePos;

    for (unsigned j = 0; j < nbItems; j++)
        list[i + j] = list[*freePos - (nbItems - j)];

    *freePos -= nbItems;
}

void upl_foreach(upl_t *const this, void (*iterator)())
{
    upl_bulk_foreach(this, iterator, 1);
}

void upl_bulk_foreach(upl_t *const this, void (*iterator)(), unsigned nbItems)
{
    void **const list = this->list;
    unsigned *const freePos = &this->freePos;

    switch (nbItems)
    {
    case 1:
        for (unsigned i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0]);
        break;
    case 2:
        for (unsigned i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1]);
        break;
    case 3:
        for (unsigned i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2]);
        break;
    case 4:
        for (unsigned i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
        break;
    case 5:
        for (unsigned i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
        break;
    case 6:
        for (unsigned i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
        break;
    case 7:
        for (unsigned i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);
        break;
    }
}

void upl_end(upl_t *const this)
{
    free(this->list);
}