#pragma once

#include "systemdef.h"

typedef struct _system
{
    const Systemdef *def;
    const void **list;
    unsigned length;
} System;

void system_init    ( System *const, const Systemdef * );
void system_update  ( System *const );
void system_destroy ( System *const );
void system_info    ( System *const );

#define system_function(FUNC, CODE) \
    void FUNC(System *const s)      \
    {                               \
        int i = 0;                  \
        int len = s->length;        \
        while ( i < len )           \
            CODE                    \
    }

#define system_get(T, V)            \
    T *const V = (T *const)s->list[i++]

#define system_add(S, A)            \
    S.list[S.length++] = A
