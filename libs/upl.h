#pragma once

typedef struct upl_t
{
    void **list;
    unsigned freePos;
    unsigned size;
} upl_t;

int  upl_init              ( upl_t *const list, unsigned size );
int  upl_add               ( upl_t *const list, void *const add );
int  upl_find              ( upl_t *const list, void *const data );
void upl_remove            ( upl_t *const list, unsigned index );
void upl_removeByData      ( upl_t *const list, void *const data );
void upl_bulk_removeByData ( upl_t *const list, void *const data, unsigned nbItems );
void upl_foreach           ( upl_t *const list, void (*iterator)() );
void upl_bulk_foreach      ( upl_t *const list, void (*iterator)(), unsigned nbItems );
void upl_end               ( upl_t *const list );
int  upl_resize            ( upl_t *const list, unsigned int size );