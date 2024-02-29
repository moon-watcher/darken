#pragma once

typedef struct uplist
{
    void **items;
    unsigned count;
    unsigned capacity;
    //
    unsigned allocations;
    unsigned item_size;
} uplist;

// void uplist_init(uplist *const, unsigned);
// int uplist_add(uplist *const, void *const);
// void *uplist_resize(uplist *const, unsigned);
// int uplist_find(uplist *const, void *const);
// void uplist_iterator(uplist *const this, void (*callback)(), unsigned params);
// unsigned uplist_remove(uplist *const, unsigned);
// unsigned uplist_removeByData(uplist *const, void *const, unsigned);
// void uplist_reset(uplist *const);
// void uplist_end(uplist *const);