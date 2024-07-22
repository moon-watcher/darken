#pragma once

#include "../../libs/uplist.h"

typedef struct
{
    uplist list;
    void (*update)();
    void (*destroy)();
} vanillamanager;

void vanillamanager_init(vanillamanager *const, unsigned, void(*), void(*));
void *vanillamanager_new(vanillamanager *const);
void vanillamanager_update(vanillamanager *const);
unsigned vanillamanager_delete(vanillamanager *const, void *const);
void vanillamanager_reset(vanillamanager *const);
void vanillamanager_end(vanillamanager *const);