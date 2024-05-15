#pragma once

#include "uplist.h"

typedef struct
{
    uplist list;
    void (*update)();
    void (*destroy)();
} vanman;

void vanman_init(vanman *const, unsigned, void(*), void(*));
void *vanman_new(vanman *const);
void vanman_update(vanman *const);
unsigned vanman_delete(vanman *const, void *const);
void vanman_reset(vanman *const);
void vanman_end(vanman *const);