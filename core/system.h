#pragma once

#include "../libs/uplist.h"

typedef struct deSystem_t deSystem_t;
typedef void (*deSystem_f)();

typedef struct deSystem_t
{
    deSystem_f updateFn;
    unsigned int maxItems;
    unsigned int params;
    uplist_t upl;
    char *name;
    void (*errorHandler)(deSystem_t *const);
} deSystem_t;

void deSystem_init         ( deSystem_t *const system, deSystem_f const updateFn, unsigned int maxItems, unsigned int params );
void deSystem_add          ( deSystem_t *const system, ... );
void deSystem_delete       ( deSystem_t *const system, void *const data );
void deSystem_update       ( deSystem_t *const system );
void deSystem_end          ( deSystem_t *const system );
void deSystem_errorHandler ( deSystem_t *const system, void (*errorHandler)(deSystem_t *const) );


// // errorHandler
//  VDP_init();
//  VDP_drawText("System: Too much params", 0, 0);
//  VDP_drawText(this->name, 0, 1);
//  waitMs(10000);
