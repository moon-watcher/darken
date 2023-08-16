#include "uplist.h"

#include "../config/free.h"
#include "../config/malloc.h"

// DarkEn List Utils

void de_common_init(uplist *const this, unsigned int size)
{
    this->size = size ? size : 1;
    this->list = malloc(this->size * sizeof(void *));
    this->freePos = 0;
}

int de_common_resize(uplist *const this, unsigned int size)
{
    if (this->size == size)
        return -1;

    unsigned int const oldSize = this->size;
    this->size = size;
    void *list = malloc(this->size * sizeof(void *));

    if (list == 0)
        return 0;

    memcpy(list, this->list, oldSize);
    free(this->list);

    this->list = list;

    return 1;
}

int de_common_find(uplist *const this, void *const data)
{
    void **const list = this->list;
    unsigned int *const freePos = &this->freePos;

    for (unsigned int i = 0; i < *freePos; i++)
        if (list[i] == data)
            return i;

    return -1;
}

void de_common_remove(uplist *const this, unsigned int index, void (*callback)(void *const))
{
    unsigned int *const freePos = &this->freePos;

    if (*freePos == 0)
        return;

    void **const list = this->list;

    if (callback)
        callback(list[index]);

    --*freePos;
    list[index] = list[*freePos];
}