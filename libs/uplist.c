#include "uplist.h"
#include "utils.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Unordered Pointers List

void uplist_init(uplist_t *const this, unsigned int size)
{
    delutil_init(this, size);
}

int uplist_add(uplist_t *const this, void *const add)
{
    unsigned int *const freePos = &this->freePos;

    if (*freePos >= this->size)
        if (delutil_resize(this, this->size + this->size / 2) == 0)
            return -1;

    this->list[*freePos] = add;
    ++*freePos;

    return *freePos - 1;
}

void uplist_iterator(uplist_t *const this, void (*iterator)(), unsigned int nbItems)
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

void uplist_remove(uplist_t *const this, unsigned int index)
{
    delutil_remove(this, index, 0);
}

void uplist_end(uplist_t *const this)
{
    free(this->list);
}

void uplist_reset(uplist_t *const this)
{
    this->freePos = 0;
}

void uplist_removeByData(uplist_t *const this, void *const data, unsigned int nbItems)
{
    int const index = delutil_find(this, data);

    if (index < 0)
        return;

    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    for (unsigned int j = 0; j < nbItems; j++)
        list[index + j] = list[*freePos - (nbItems - j)];

    *freePos -= nbItems;
}