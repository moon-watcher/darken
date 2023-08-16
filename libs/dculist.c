#include "dculist.h"
// #include "common.h"

#include "../config/free.h"
#include "../config/malloc.h"

// Dynamic Chacheable Unordered List

void dculist_init(dculist *const this, unsigned int size, unsigned int objectSize)
{
    uplist *const upl = &this->upl;
    
    uplist_init(upl, size);

    this->objectSize = objectSize ? objectSize : 1;
    this->allocatedObjects = 0;
}

void *dculist_add(dculist *const this)
{
    uplist *const upl = &this->upl;
    unsigned int next = upl->next;

    if (next < this->allocatedObjects)
        ++upl->next;
    else
    {
        int pos = uplist_add(upl, malloc(this->objectSize));

        if (pos < 0)
            return 0; 
            
        next = pos;
        ++this->allocatedObjects;
    }
    
    return upl->list[next];
}

void dculist_iterator(dculist *const this, void (*callback)(void *const))
{
    if (callback == 0)
        return;

    uplist_iterator(&this->upl, callback, 1);
}

void dculist_remove(dculist *const this, void *const data, void (*callback)(void *const))
{
    uplist *const upl = &this->upl;
    int const index = uplist_find(upl, data);

    if (index < 0)
        return;

    if(callback)
        callback(upl->list[index]);
        
    uplist_remove(upl, index);
}

void dculist_end(dculist *const this, void (*callback)(void *const))
{
    dculist_reset(this, callback);

    uplist *const upl = &this->upl;
    void **const list = upl->list;

    for (unsigned int i = 0; i < this->allocatedObjects; i++)
        free(list[i]);

    this->allocatedObjects = 0;

    uplist_end(upl);
}

void dculist_reset(dculist *const this, void (*callback)(void *const))
{
    uplist *const upl = &this->upl;

    dculist_iterator(this, callback);
    uplist_reset(upl);
}