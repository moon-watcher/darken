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

#define _SYS_FOREACH(SYSTEM)           \
    void **items = SYSTEM->list.items; \
    uint16_t size = SYSTEM->list.size; \
    uint16_t params = SYSTEM->params;  \
    for (uint16_t i = 0; i < size; i += params)

#define _SYS_GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define DE_SYSTEM_FOREACH(...)     \
    _SYS_GET_MACRO(__VA_ARGS__,    \
                   _SYS_ITERATE_4, \
                   _SYS_ITERATE_3, \
                   _SYS_ITERATE_2, \
                   _SYS_ITERATE_1, \
                   _SYS_ITERATE_0)(__VA_ARGS__)

#define _SYS_ITERATE_0(SYSTEM, IT) \
    _SYS_FOREACH(SYSTEM)           \
    {                              \
        IT;                        \
    }

#define _SYS_ITERATE_1(SYSTEM, A, IT) \
    _SYS_FOREACH(SYSTEM)              \
    {                                 \
        A = items[i];                 \
        IT;                           \
    }

#define _SYS_ITERATE_2(SYSTEM, A, B, IT) \
    _SYS_FOREACH(SYSTEM)                 \
    {                                    \
        A = items[i];                    \
        B = items[i + 1];                \
        IT;                              \
    }

#define _SYS_ITERATE_3(SYSTEM, A, B, C, IT) \
    _SYS_FOREACH(SYSTEM)                    \
    {                                       \
        A = items[i];                       \
        B = items[i + 1];                   \
        C = items[i + 2];                   \
        IT;                                 \
    }

#define _SYS_ITERATE_4(SYSTEM, A, B, C, D, IT) \
    _SYS_FOREACH(SYSTEM)                       \
    {                                          \
        A = items[i];                          \
        B = items[i + 1];                      \
        C = items[i + 2];                      \
        D = items[i + 3];                      \
        IT;                                    \
    }
