#pragma once

#include "../libs/uplist.h"

typedef struct
{
    uplist list;
    void (*update)();
    void (*destroy)();
} vanilla;

void vanilla_init(vanilla *const, unsigned, void(*), void(*));
void *vanilla_new(vanilla *const);
void vanilla_update(vanilla *const);
unsigned vanilla_delete(vanilla *const, void *const);
void vanilla_reset(vanilla *const);
void vanilla_end(vanilla *const);