// Unordered Cacheable List

#include "uclist.h"
#include "config.h"

static void *_resize(uclist *const this)
{
    void *ptr = malloc((this->capacity + 1) * sizeof(void *));

    if (ptr != 0)
    {
        memcpy(ptr, this->list, this->capacity * sizeof(void *));
        free(this->list);
        this->list = ptr;
        ++this->capacity;
    }

    return ptr;
}

//

void uclist_init(uclist *const this, unsigned maxItemSize)
{
    memset(this, 0, sizeof(uclist));
    this->itemSize = maxItemSize;
}

void *uclist_alloc(uclist *const this)
{
    void *ptr = 0;

    if (this->count < this->capacity)
    {
        ptr = this->list[this->count++];
    }
    else if ((ptr = malloc(this->itemSize)) != 0)
    {
        ptr = uclist_add(this, ptr);
    }

    memset(ptr, 0, this->itemSize);

    return ptr;
}

void *uclist_add(uclist *const this, void *const add)
{
    if (this->count >= this->capacity && _resize(this) == 0)
    {
        return 0;
    }

    return this->list[this->count++] = add;
}

int uclist_find(uclist *const this, void *const data)
{
    int count = (int)this->count;

    while (count--)
    {
        if (this->list[count] == data)
        {
            return count;
        }
    }

    return UCLIST_NOT_FOUND;
}

int uclist_iterator(uclist *const this, void (*iterator)())
{
    if (iterator == 0)
    {
        return UCLIST_NO_ITERATOR;
    }

    for (unsigned i = 0; i < this->count; ++i)
    {
        iterator(this->list[i]);
    }

    return this->count;
}

int uclist_remove(uclist *const this, void *const data, void (*exec)())
{
    int index = uclist_find(this, data);

    if (index >= 0)
    {
        uclist_removeByIndex(this, index, exec);
    }

    return index;
}

void uclist_removeByIndex(uclist *const this, unsigned index, void (*exec)())
{
    if (this->count == 0)
    {
        return;
    }
    else if (exec != 0)
    {
        exec(this->list[index]);
    }

    --this->count;

    void *const swap = this->list[index];
    this->list[index] = this->list[this->count];
    this->list[this->count] = swap;
}

void uclist_reset(uclist *const this)
{
    this->count = 0;
}

void uclist_end(uclist *const this)
{
    while (this->itemSize && this->capacity--)
    {
        free(this->list[this->capacity]);
    }

    free(this->list);
    uclist_init(this, this->itemSize);
}
