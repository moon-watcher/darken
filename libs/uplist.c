// Unordered list of pointers

#include "uplist.h"
#include "config.h"

void uplist_init(uplist *const this)
{
    memset(this, 0, sizeof(uplist));
}

void uplist_initAlloc(uplist *const this, unsigned itemSize)
{
    uplist_init(this);
    this->itemSize = itemSize;
}

void *uplist_alloc(uplist *const this)
{
    if (this->count < this->capacity)
    {
        unsigned count = this->count++;
        memset(this->items[count], 0, this->itemSize);

        return this->items[count];
    }

    void *ptr = malloc(this->itemSize);

    if (ptr == 0)
        return 0;

    memset(ptr, 0, this->itemSize);

    void *added = uplist_add(this, ptr);

    if (added == 0)
        free(ptr);

    return added;
}

void *uplist_add(uplist *const this, void *const add)
{
    if (this->count >= this->capacity)
    {
        void *ptr = malloc((this->capacity + 1) * sizeof(void *));

        if (ptr == 0)
            return 0;

        memcpy(ptr, this->items, this->capacity * sizeof(void *));
        free(this->items);
        this->items = ptr;
        ++this->capacity;
    }

    return this->items[this->count++] = add;
}

int uplist_find(uplist *const this, void *const data)
{
    for (unsigned i = 0; i < this->count; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}

void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
{
    unsigned const count = this->count;

    if (nbItems == 0 || iterator == 0 || count == 0)
        ;
    else if (nbItems == 1)
        for (unsigned i = 0; i < count; i += nbItems)
            iterator(this->items[i + 0]);
    else if (nbItems == 2)
        for (unsigned i = 0; i < count; i += nbItems)
            iterator(this->items[i + 0], this->items[i + 1]);
    else if (nbItems == 3)
        for (unsigned i = 0; i < count; i += nbItems)
            iterator(this->items[i + 0], this->items[i + 1], this->items[i + 2]);
    else if (nbItems == 4)
        for (unsigned i = 0; i < count; i += nbItems)
            iterator(this->items[i + 0], this->items[i + 1], this->items[i + 2], this->items[i + 3]);
    else if (nbItems == 5)
        for (unsigned i = 0; i < count; i += nbItems)
            iterator(this->items[i + 0], this->items[i + 1], this->items[i + 2], this->items[i + 3], this->items[i + 4]);
    else if (nbItems == 6)
        for (unsigned i = 0; i < count; i += nbItems)
            iterator(this->items[i + 0], this->items[i + 1], this->items[i + 2], this->items[i + 3], this->items[i + 4], this->items[i + 5]);
}

unsigned uplist_remove(uplist *const this, void *const data)
{
    int index = uplist_find(this, data);

    if (index < 0 || this->count == 0)
        return 0;

    this->items[index] = this->items[--this->count];

    return 1;
}

void uplist_removeByIndex(uplist *const this, unsigned index)
{
    this->items[index] = this->items[--this->count];
}

void uplist_reset(uplist *const this)
{
    this->count = 0;
}

void uplist_end(uplist *const this)
{
    if (this->itemSize)
        for (unsigned i = 0; i < this->capacity; i++)
            free(this->items[i]);

    free(this->items);
    uplist_initAlloc(this, this->itemSize);
}