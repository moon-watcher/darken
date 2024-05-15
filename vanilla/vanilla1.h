#pragma once

#include "../libs/uplist.h"

typedef struct
{
    uplist list;
    void (*update)();
    void (*destroy)();
} vanilla1;

void vanilla1_init(vanilla1 *const, unsigned, void(*), void(*));
void *vanilla1_new(vanilla1 *const);
void vanilla1_update(vanilla1 *const);
unsigned vanilla1_delete(vanilla1 *const, void *const);
void vanilla1_reset(vanilla1 *const);
void vanilla1_end(vanilla1 *const);