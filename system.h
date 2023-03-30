#pragma once

#include "darken.h"

typedef struct deSystem_t
{
    const void **list;
    unsigned length;
    void (*updateFn)();
    unsigned maxItems;
    unsigned params;
} deSystem_t;

void deSystem_init   ( deSystem_t *const, void (*)(), unsigned, unsigned);
void deSystem_update ( deSystem_t *const );
void deSystem_end    ( deSystem_t *const );
void deSystem_info   ( deSystem_t *const );
void deSystem_add    ( deSystem_t *const, void *const[] );

#define deSystem_define_function(FUNC, CODE) \
    void FUNC(deSystem_t *const s)  \
    {                               \
        int i = 0;                  \
        int len = s->length;        \
        while (i < len)             \
            CODE                    \
    }

#define deSystem_get(T, V) \
    T *const V = (T *const)s->list[i++]
    