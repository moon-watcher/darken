#pragma once

#include "../libs/upl.h"

typedef struct deSystem_t deSystem_t;
typedef void (*deSystem_f)();

typedef struct deSystem_t
{
    deSystem_f updateFn;
    unsigned maxItems;
    unsigned params;
    upl_t upl;
    char *name;
} deSystem_t;

void deSystem_init   ( deSystem_t *const system, deSystem_f const updateFn, unsigned maxItems, unsigned params );
void deSystem_add    ( deSystem_t *const system, ... );
void deSystem_delete ( deSystem_t *const system, void *const data );
void deSystem_update ( deSystem_t *const system );
void deSystem_end    ( deSystem_t *const system );
