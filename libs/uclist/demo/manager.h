#pragma once

#include "../uclist.h"

typedef struct
{
    uclist list;
    void (*update)();
    void (*destroy)();
} manager;

void manager_init(manager *const, unsigned, void(*), void(*));
void *manager_new(manager *const);
void manager_update(manager *const);
int manager_delete(manager *const, void *const);
void manager_reset(manager *const);
void manager_end(manager *const);