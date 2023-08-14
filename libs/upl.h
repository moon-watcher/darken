#pragma once

typedef struct upl_t
{
    void **list;
    unsigned int freePos;
    unsigned int size;
} upl_t;

void upl_init(upl_t *const list, unsigned int size);
int upl_add(upl_t *const list, void *const add);
void upl_iterator(upl_t *const list, void (*iterator)(), unsigned int nbItems);
void upl_remove(upl_t *const list, unsigned int index);
void upl_end(upl_t *const list);
void upl_reset(upl_t *const list);
void upl_removeByData(upl_t *const list, void *const data, unsigned int nbItems);

int upl_resize(upl_t *const list, unsigned int size);
int upl_find(upl_t *const list, void *const data);
