#include "dculist.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Dynamic Cached Unordered List

void dculist_init(dculist_t *const this, unsigned int maxObjects, unsigned int objectSize)
{
    this->objectSize = objectSize ? objectSize : 1;
    this->maxObjects = maxObjects ? maxObjects : 1;
    this->freePos = 0;
    this->allocatedObjects = 0;

    unsigned int listSize = sizeof(void *) * this->maxObjects;

    this->list = malloc(listSize);
    // memset(this->list, 0, listSize);
}

void dculist_reset(dculist_t *const this)
{
    this->freePos = 0;
}

void *dculist_add(dculist_t *const this)
{
    if (this->freePos >= this->maxObjects)
        return 0;

    if (this->freePos >= this->allocatedObjects)
    {
        this->list[this->freePos] = malloc(this->objectSize);
        ++this->allocatedObjects;
    }

    ++this->freePos;
    return this->list[this->freePos - 1];
}

void dculist_remove(dculist_t *const this, unsigned int index)
{
    if (index < this->freePos)
        this->list[index] = this->list[--this->freePos];
}

void dculist_end(dculist_t *const this)
{
    for (unsigned int i = 0; i < this->allocatedObjects; i++)
        free(this->list[i]);

    this->objectSize = 0;
    this->maxObjects = 0;
    this->freePos = 0;
    this->allocatedObjects = 0;

    free(this->list);
}


void dculist_iterator(dculist_t *const this, void (*iterator)(void *))
{
    for (unsigned int i = 0; i < this->freePos; i++)
        iterator(&this->list[i]);
}

// int dculist_find(dculist_t *const this, void *const data)
// {
//     void **const list = this->list;
//     unsigned int *const freePos = &this->freePos;

//     for (unsigned int i = 0; i < *freePos; i++)
//         if (list[i] == data)
//             return i;

//     return -1;
// }

// void dculist_remove(dculist_t *const this, unsigned int index)
// {
//     unsigned int *const freePos = &this->freePos;

//     if (*freePos == 0)
//         return;

//     void **const list = this->list;

//     --*freePos;
//     list[index] = list[*freePos];
// }

// void dculist_removeByData(dculist_t *const this, void *const data)
// {
//     int i = dculist_find(this, data);

//     if (i < 0)
//         return;

//     dculist_remove(this, i);
// }

// void dculist_bulk_removeByData(dculist_t *const this, void *const data, unsigned int nbItems)
// {
//     int i = dculist_find(this, data);

//     if (i < 0)
//         return;

//     void **const list = this->list;
//     unsigned int *const freePos = &this->freePos;

//     for (unsigned int j = 0; j < nbItems; j++)
//         list[i + j] = list[*freePos - (nbItems - j)];

//     *freePos -= nbItems;
// }

// void dculist_foreach(dculist_t *const this, void (*iterator)())
// {
//     dculist_bulk_foreach(this, iterator, 1);
// }

// void dculist_bulk_foreach(dculist_t *const this, void (*iterator)(), unsigned int nbItems)
// {
//     void **const list = this->list;
//     unsigned int *const freePos = &this->freePos;

//     switch (nbItems)
//     {
//     case 1:
//         for (unsigned int i = 0; i < *freePos; i += nbItems)
//             iterator(list[i + 0]);
//         break;
//     case 2:
//         for (unsigned int i = 0; i < *freePos; i += nbItems)
//             iterator(list[i + 0], list[i + 1]);
//         break;
//     case 3:
//         for (unsigned int i = 0; i < *freePos; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2]);
//         break;
//     case 4:
//         for (unsigned int i = 0; i < *freePos; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
//         break;
//     case 5:
//         for (unsigned int i = 0; i < *freePos; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
//         break;
//     case 6:
//         for (unsigned int i = 0; i < *freePos; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
//         break;
//     case 7:
//         for (unsigned int i = 0; i < *freePos; i += nbItems)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);
//         break;
//     }
// }

// int dculist_resize(dculist_t *const this, unsigned int size)
// {
//     if (this->size == size)
//         return -1;

//     unsigned int oldSize = this->size;
//     this->size = size;
//     void *list = malloc(this->size * sizeof(void *));

//     if (list == 0)
//         return 0;

//     memcpy(list, this->list, oldSize);
//     free(this->list);

//     this->list = list;

//     return 1;
// }
