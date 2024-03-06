// Unordered list of pointers

#include "uplist.h"

#include "../services/free.h"
#include "../services/malloc.h"

void uplist_init(uplist *const this)
{
    this->items = 0;
    this->count = 0;
    this->capacity = 0;
    this->resizeBy = 1;
}

void *uplist_add(uplist *const this, void *const add)
{
    if (this->count >= this->capacity && uplist_resize(this, this->resizeBy) == 0)
        return 0;

    return this->items[this->count++] = add;
}

unsigned uplist_resize(uplist *const this, unsigned increment)
{
    unsigned size = this->capacity * sizeof(void *);
    this->capacity += increment ?: this->resizeBy ?: 1;
    void *ptr = malloc(this->capacity * sizeof(void *));

    if (ptr == 0)
        return 0;

    memcpy(ptr, this->items, size);
    free(this->items);
    this->items = ptr;

    return 1;
}

int uplist_find(uplist *const this, void *const data)
{
    for (unsigned i = 0; i < this->count; i++)
        if (this->items[i] == data)
            return i;

    return -1;
}

unsigned uplist_remove(uplist *const this, void *const data)
{
    int index = uplist_find(this, data);

    if (index < 0)
        return 0;

    return uplist_removeById(this, index);
}

unsigned uplist_removeById(uplist *const this, unsigned index)
{
    if (this->count == 0)
        return 0;

    this->items[index] = this->items[--this->count];

    return 1;
}

void uplist_reset(uplist *const this)
{
    this->count = 0;
}

void uplist_end(uplist *const this)
{
    free(this->items);
    uplist_init(this);
}









// // Unordered list of pointers

// #include "uplist.h"

// #include "../services/free.h"
// #include "../services/malloc.h"

// void uplist_init(uplist *const this)
// {
//     this->items = 0;
//     this->count = 0;
//     this->capacity = 0;
//     this->resizeBy = 1;
// }

// void *uplist_add(uplist *const this, void *const add)
// {
//     if (this->count >= this->capacity && uplist_resize(this, this->resizeBy) == 0)
//         return 0;

//     return this->items[this->count++] = add;
// }

// unsigned uplist_resize(uplist *const this, unsigned increment)
// {
//     unsigned size = this->capacity * sizeof(void *);
//     this->capacity += increment ?: this->resizeBy ?: 1;
//     void *ptr = malloc(this->capacity * sizeof(void *));

//     if (ptr == 0)
//         return 0;

//     memcpy(ptr, this->items, size);
//     free(this->items);
//     this->items = ptr;

//     return 1;
// }

// int uplist_find(uplist *const this, void *const data)
// {
//     for (unsigned i = 0; i < this->count; i++)
//         if (this->items[i] == data)
//             return i;

//     return -1;
// }

// unsigned uplist_remove(uplist *const this, void *const data)
// {
//     int index = uplist_find(this, data);

//     if (index < 0)
//         return 0;

//     return uplist_removeById(this, index);
// }

// unsigned uplist_removeById(uplist *const this, unsigned index)
// {
//     if (this->count == 0)
//         return 0;

//     this->items[index] = this->items[--this->count];

//     return 1;
// }

// void uplist_reset(uplist *const this)
// {
//     this->count = 0;
// }

// void uplist_end(uplist *const this)
// {
//     free(this->items);
//     uplist_init(this);
// }