#pragma once

typedef struct uplist_t
{
    void **list;
    unsigned int freePos;
    unsigned int size;
} uplist_t;

void uplist_init(uplist_t *const list, unsigned int size);
int uplist_add(uplist_t *const list, void *const add);
void uplist_iterator(uplist_t *const list, void (*iterator)(), unsigned int nbItems);
void uplist_remove(uplist_t *const list, unsigned int index);
void uplist_end(uplist_t *const list);
void uplist_reset(uplist_t *const list);
void uplist_removeByData(uplist_t *const list, void *const data, unsigned int nbItems);

int uplist_resize(uplist_t *const list, unsigned int size);
int uplist_find(uplist_t *const list, void *const data);
