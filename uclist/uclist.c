// Unordered Cacheable List

#include "uclist.h"
#include "config.h"

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
        ptr = this->list[this->count];
        ++this->count;
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
    if (add == 0)
    {
        return 0;
    }
    else if (this->count >= this->capacity)
    {
        void *ptr = malloc((this->capacity + 1) * sizeof(void *));

        if (ptr == 0)
        {
            return 0;
        }

        memcpy(ptr, this->list, this->capacity * sizeof(void *));
        free(this->list);
        this->list = ptr;
        ++this->capacity;
    }

    this->list[this->count] = add;
    ++this->count;

    return add;
}

int uclist_iterator(uclist *const this, void (*iterator)())
{
    if (this->count == 0)
    {
        return 0;
    }
    else if (iterator == 0)
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
    int index = (int)this->count;

    while (index--)
    {
        void *const ptr = this->list[index];

        if (ptr == data)
        {
            if (exec != 0)
            {
                exec(ptr);
            }

            --this->count;

            this->list[index] = this->list[this->count];
            this->list[this->count] = ptr;

            return index;
        }
    }

    return UCLIST_NOT_FOUND;
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
