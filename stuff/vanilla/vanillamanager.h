#pragma once

#include "../../libs/uclist.h"

typedef struct
{
    uclist list;
    void (*update)();
    void (*destroy)();
} vanillamanager;

void vanillamanager_init(vanillamanager *const, unsigned, void(*), void(*));
void *vanillamanager_new(vanillamanager *const);
void vanillamanager_update(vanillamanager *const);
int vanillamanager_delete(vanillamanager *const, void *const);
void vanillamanager_reset(vanillamanager *const);
void vanillamanager_end(vanillamanager *const);