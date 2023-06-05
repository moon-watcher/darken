#pragma once

typedef struct UFL_t
{
    void **list;
    unsigned freePos;
    unsigned maxElements;
} UFL_t;

void UFL_init(UFL_t *const this, unsigned maxElements);
int UFL_add(UFL_t *const this, void *const add);
int UFL_removeByIndex(UFL_t *const this, unsigned index);
int UFL_removeByData(UFL_t *const this, void *const data);
int UFL_removeByDataBulk(UFL_t *const this, void *const data, unsigned nbItems);
void UFL_foreach(UFL_t *const this, void (*iterator)());
void UFL_foreachBulk(UFL_t *const this, void (*iterator)(), unsigned nbItems);
void UFL_end(UFL_t *const this);
