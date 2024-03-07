#pragma once

#include "uplist.h"

typedef struct culist
{
    uplist upl;
    unsigned itemSize;
} culist;

void culist_init(culist *const, unsigned);
void *culist_add(culist *const);
void culist_end(culist *const);

#define culist_find(this, data) uplist_find(this.upl, data)
#define culist_iterator(this, iterator, nbItems) uplist_iterator(this.upl, iterator, nbItems)
#define culist_remove(this, data) uplist_remove(this.upl, data)
#define culist_removeById(this, index) uplist_removeById(this.upl, index)
#define culist_reset(this) uplist_reset(this.upl)