#pragma once

#include <stdint.h>

typedef struct
{
    void **items;
    uint16_t size;
    uint16_t capacity;
    uint16_t itemSize;
    uint8_t mode;
} uclist;

void uclist_init_add(uclist *);
void uclist_init_alloc(uclist *, uint16_t);
uint16_t uclist_init_fixedAlloc(uclist *, uint16_t, uint16_t);
void *uclist_alloc(uclist *);
void *uclist_add(uclist *, void *);
void *uclist_addUnsafe(uclist *, void *);
void uclist_iterator(uclist *, void (*)());
int16_t uclist_getIndex(uclist *, void *);
uint16_t uclist_removeByIndex(uclist *, uint16_t);
uint16_t uclist_remove(uclist *, void *);
uint16_t uclist_restore(uclist *, void *);
void uclist_reset(uclist *);
void uclist_end(uclist *);

//

uint16_t uclist_iteratorEx(uclist *, void (*)(), uint16_t);
