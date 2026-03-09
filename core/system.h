#pragma once

#include <stdint.h>
#include "list.h"

typedef struct de_system de_system;

typedef struct de_system
{
    de_list list;
    void *(*update)(de_system *);
    uint16_t params;
} de_system;

void de_system_init(de_system *, void *(*)(), uint16_t);
uint16_t de_system_add(de_system *, void *[]);
uint16_t de_system_addFast(de_system *, void *[]);
uint16_t de_system_delete(de_system *, void *);
void de_system_reset(de_system *);
void de_system_end(de_system *);

#define DE_SYSTEM_FOREACH(...) _SYS_GET_MACRO(__VA_ARGS__, _SYS_FOREACH_5, _SYS_FOREACH_4, _SYS_FOREACH_3, _SYS_FOREACH_2, _SYS_FOREACH_1, _SYS_FOREACH_0)(__VA_ARGS__)
#define DE_SYSTEM_ITERATOR(...) _SYS_GET_MACRO(__VA_ARGS__, _SYS_ITERATOR_5, _SYS_ITERATOR_4, _SYS_ITERATOR_3, _SYS_ITERATOR_2, _SYS_ITERATOR_1, _SYS_ITERATOR_0)(__VA_ARGS__)

//

#define _SYS_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME

#define _SYS_FOREACH(SYSTEM, IT)                              \
    void **items = SYSTEM->list.items;                        \
    for (uint16_t i = 0, size = SYSTEM->list.size; i < size;) \
        IT;

//

#define _SYS_FOREACH_0(SYSTEM, IT) \
    _SYS_FOREACH(SYSTEM, { IT; })

#define _SYS_FOREACH_1(SYSTEM, A, IT) \
    _SYS_FOREACH(SYSTEM, { A = items[i++]; IT; })

#define _SYS_FOREACH_2(SYSTEM, A, B, IT) \
    _SYS_FOREACH(SYSTEM, { A = items[i++]; B = items[i++]; IT; })

#define _SYS_FOREACH_3(SYSTEM, A, B, C, IT) \
    _SYS_FOREACH(SYSTEM, { A = items[i++]; B = items[i++]; C = items[i++]; IT; })

#define _SYS_FOREACH_4(SYSTEM, A, B, C, D, IT) \
    _SYS_FOREACH(SYSTEM, { A = items[i++]; B = items[i++]; C = items[i++]; D = items[i++]; IT; })

#define _SYS_FOREACH_5(SYSTEM, A, B, C, D, E, IT) \
    _SYS_FOREACH(SYSTEM, { A = items[i++]; B = items[i++]; C = items[i++]; D = items[i++]; E = items[i++]; IT; })

//

#define _SYS_ITERATOR_0(NAME, IT) \
    void NAME(de_system *system) { _SYS_FOREACH_0(system, IT); }

#define _SYS_ITERATOR_1(NAME, A, IT) \
    void NAME(de_system *system) { _SYS_FOREACH_1(system, A, IT); }

#define _SYS_ITERATOR_2(NAME, A, B, IT) \
    void NAME(de_system *system) { _SYS_FOREACH_2(system, A, B, IT); }

#define _SYS_ITERATOR_3(NAME, A, B, C, IT) \
    void NAME(de_system *system) { _SYS_FOREACH_3(system, A, B, C, IT); }

#define _SYS_ITERATOR_4(NAME, A, B, C, D, IT) \
    void NAME(de_system *system) { _SYS_FOREACH_4(system, A, B, C, D, IT); }

#define _SYS_ITERATOR_5(NAME, A, B, C, D, E, IT) \
    void NAME(de_system *system) { _SYS_FOREACH_5(system, A, B, C, D, E, IT); }
