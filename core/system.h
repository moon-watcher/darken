#pragma once

#include <stdint.h>
#include "list.h"

typedef struct de_system de_system;
typedef struct de_system
{
    de_list list;
    void *(*update)(de_system *);
    uint16_t params;
    void *response;
    void *extraData;
    //
    // char *name;
} de_system;

void de_system_init(de_system *, void *(*)(), uint16_t);
uint16_t de_system_add(de_system *, void *[]);
uint16_t de_system_addUnsafe(de_system *, void *[]);
uint16_t de_system_delete(de_system *, void *);
void de_system_update(de_system *);
void de_system_reset(de_system *);
void de_system_end(de_system *);

//

#define DE_SYSTEM_FOREACH(ITERABLE)             \
    void **items = system->list.items;          \
    uint16_t size = system->list.size;          \
    uint16_t params = system->params;           \
                                                \
    for (uint16_t i = 0; i < size; i += params) \
    {                                           \
        ITERABLE                                \
    }

#define DE_SYSTEM_ITERATE(NAME, ITERABLE) \
    void NAME(de_system *system)          \
    {                                     \
        DE_SYSTEM_FOREACH(ITERABLE)       \
    }

#define DE_SYSTEM_PARAM(PARAM) items[i + PARAM]
