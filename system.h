#pragma once

#include "config.h"

typedef uclist de_system_list_t;

typedef struct
{
    de_system_list_t list;
    void (*update_f)();
    uint16_t params;

    // debug purposes
    char *name;
} de_system_t;

void de_system_init(de_system_t *, void (*)(), uint16_t);
uint16_t de_system_add(de_system_t *, void *[]);
uint16_t de_system_delete(de_system_t *, void *);
void de_system_updateLoopExpand(de_system_t *);
void de_system_updateLoop(de_system_t *);
void de_system_reset(de_system_t *);
void de_system_end(de_system_t *);

//

void de_system_addUnsafe(de_system_t *, void *[]);

#define DARKSYS_DECLARE(NAME) void NAME(de_system_list_t *, uint16_t)

#define DARKSYS_ITERATE(NAME, ITERABLE)              \
    void NAME(de_system_list_t *list, uint16_t params) \
    {                                                \
        void **items = list->items;                  \
        uint16_t size = list->size;                  \
                                                     \
        for (uint16_t i = 0; i < size; i += params)  \
            ITERABLE                                 \
    }

#define DARKSYS_PARAM(PARAM) items[i + PARAM]

void de_system_update(de_system_t *);
