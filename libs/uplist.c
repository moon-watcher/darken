#include "uplist.h"
#include "common.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Unordered Pointers List

void uplist_init(uplist *const this, unsigned int size)
{
    de_common_init(this, size);
}

int uplist_add(uplist *const this, void *const add)
{
    unsigned int *const freePos = &this->freePos;

    if (*freePos >= this->size)
        if (de_common_resize(this, this->size + this->size / 2) == 0)
            return -1;

    this->list[*freePos] = add;
    ++*freePos;

    return *freePos - 1;
}

void uplist_iterator(uplist *const this, void (*iterator)(), unsigned int nbItems)
{
    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    switch (nbItems)
    {
    case 1:
        for (unsigned int i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0]);
        break;
    case 2:
        for (unsigned int i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1]);
        break;
    case 3:
        for (unsigned int i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2]);
        break;
    case 4:
        for (unsigned int i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
        break;
    case 5:
        for (unsigned int i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
        break;
    case 6:
        for (unsigned int i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
        break;
    case 7:
        for (unsigned int i = 0; i < *freePos; i += nbItems)
            iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);
        break;
    }
}

void uplist_remove(uplist *const this, unsigned int index)
{
    de_common_remove(this, index, 0);
}

void uplist_end(uplist *const this)
{
    free(this->list);
}

void uplist_reset(uplist *const this)
{
    this->freePos = 0;
}

void uplist_removeByData(uplist *const this, void *const data, unsigned int nbItems)
{
    int const index = de_common_find(this, data);

    if (index < 0)
        return;

    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    for (unsigned int j = 0; j < nbItems; j++)
        list[index + j] = list[*freePos - (nbItems - j)];

    *freePos -= nbItems;
}