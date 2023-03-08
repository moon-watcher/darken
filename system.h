#pragma once

#include "darken.h"

typedef struct deSystem_t
{
    const deDefinition_t *definition;
    const void **list;
    unsigned length;
} deSystem_t;

void de_system_init    ( deSystem_t *const, const deDefinition_t * );
void de_system_update  ( deSystem_t *const );
void de_system_end ( deSystem_t *const );
void de_system_info    ( deSystem_t *const );

#define system_function(FUNC, CODE) \
    void FUNC(deSystem_t *const s)      \
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
