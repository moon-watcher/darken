#include <genesis.h>
#include "UFL.h"

void UFL_init(UFL_t *const this, unsigned maxElements)
{
    this->list = malloc(maxElements * sizeof(void *));
    this->freePos = 0;
    this->maxElements = maxElements;
}

int UFL_add(UFL_t *const this, void *const add)
{
    if (this->freePos >= this->maxElements)
        return -1;

    this->list[this->freePos] = add;
    this->freePos++;

    return this->freePos - 1;
}

int UFL_removeByIndex(UFL_t *const this, unsigned index)
{
    if (this->freePos == 0)
        return 0;

    --this->freePos;
    this->list[index] = this->list[this->freePos];

    return 1;
}

int UFL_removeByData(UFL_t *const this, void *const data)
{
    for (unsigned i = 0; i < this->freePos; i++)
        if (this->list[i] == data)
            return UFL_removeByIndex(this, i);

    return 0;
}

int UFL_bulk_removeByData(UFL_t *const this, void *const data, unsigned nbItems)
{
    for (unsigned i = 0; i < this->freePos; i++)
        if (this->list[i] == data)
        {
            for (unsigned j = 0; j < nbItems; j++)
                this->list[i + j] = this->list[this->freePos - (nbItems - j)];

            this->freePos -= nbItems;

            return 1;
        }

    return 0;
}

void UFL_foreach(UFL_t *const this, void (*iterator)())
{
    UFL_bulk_foreach(this, iterator, 1);
}

void UFL_bulk_foreach(UFL_t *const this, void (*iterator)(), unsigned nbItems)
{
    for (unsigned i = 0; i < this->freePos; i += nbItems)
        iterator(
            this->list[i + 0],
            this->list[i + 1],
            this->list[i + 2],
            this->list[i + 3],
            this->list[i + 4],
            this->list[i + 5],
            this->list[i + 6]);
}

void UFL_end(UFL_t *const this)
{
    free(this->list);
}