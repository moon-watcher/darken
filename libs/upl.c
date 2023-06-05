#include <genesis.h>
#include "upl.h"

// Unordered Pointers List

void upl_init(upl_t *const this, unsigned maxElements)
{
    this->list = malloc(maxElements * sizeof(void *));
    this->freePos = 0;
    this->maxElements = maxElements;
}

int upl_add(upl_t *const this, void *const add)
{
    void **const list = this->list;
    unsigned *const freePos = &this->freePos;

    if (*freePos >= this->maxElements)
        return -1;

    list[*freePos] = add;
    ++*freePos;

    return *freePos - 1;
}

int upl_removeByIndex(upl_t *const this, unsigned index)
{
    void **const list = this->list;
    unsigned *const freePos = &this->freePos;
    
    if (*freePos == 0)
        return 0;

    --*freePos;
    list[index] = list[*freePos];

    return 1;
}

int upl_removeByData(upl_t *const this, void *const data)
{
    void **const list = this->list;
    unsigned *const freePos = &this->freePos;
    
    for (unsigned i = 0; i < *freePos; i++)
        if (list[i] == data)
            return upl_removeByIndex(this, i);

    return 0;
}

int upl_bulk_removeByData(upl_t *const this, void *const data, unsigned nbItems)
{
    void **const list = this->list;
    unsigned *const freePos = &this->freePos;

    for (unsigned i = 0; i < *freePos; i++)
        if (list[i] == data)
        {
            for (unsigned j = 0; j < nbItems; j++)
                list[i + j] = list[*freePos - (nbItems - j)];

            *freePos -= nbItems;

            return 1;
        }

    return 0;
}

void upl_foreach(upl_t *const this, void (*iterator)())
{
    upl_bulk_foreach(this, iterator, 1);
}

void upl_bulk_foreach(upl_t *const this, void (*iterator)(), unsigned nbItems)
{
    void **const list = this->list;
    unsigned *const freePos = &this->freePos;

    for (unsigned i = 0; i < *freePos; i += nbItems)
        iterator(
            list[i + 0],
            list[i + 1],
            list[i + 2],
            list[i + 3],
            list[i + 4],
            list[i + 5],
            list[i + 6]);
}

void upl_end(upl_t *const this)
{
    free(this->list);
}