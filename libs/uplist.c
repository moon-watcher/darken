#include "uplist.h"
#include "common.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Unordered Pointers List

void de_libs_uplist_init(de_libs_uplist *const this, unsigned int size)
{
    de_libs_common_init(this, size);
}

int de_libs_uplist_add(de_libs_uplist *const this, void *const add)
{
    unsigned int *const freePos = &this->freePos;

    if (*freePos >= this->size)
        if (de_libs_common_resize(this, this->size + this->size / 2) == 0)
            return -1;

    this->list[*freePos] = add;
    ++*freePos;

    return *freePos - 1;
}

void de_libs_uplist_iterator(de_libs_uplist *const this, void (*iterator)(), unsigned int nbItems)
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

void de_libs_uplist_remove(de_libs_uplist *const this, unsigned int index)
{
    de_libs_common_remove(this, index, 0);
}

void de_libs_uplist_end(de_libs_uplist *const this)
{
    free(this->list);
}

void de_libs_uplist_reset(de_libs_uplist *const this)
{
    this->freePos = 0;
}

void de_libs_uplist_removeByData(de_libs_uplist *const this, void *const data, unsigned int nbItems)
{
    int const index = de_libs_common_find(this, data);

    if (index < 0)
        return;

    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    for (unsigned int j = 0; j < nbItems; j++)
        list[index + j] = list[*freePos - (nbItems - j)];

    *freePos -= nbItems;
}