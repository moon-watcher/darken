#pragma once

#include "../libs/uplist.h"

typedef struct de_manager
{
    uplist list;
    void (*update)();
    void (*destroy)();
} de_manager;

void de_manager_initEx(de_manager *const, unsigned, void(*), void (*));
void *de_manager_new(de_manager *const);
void de_manager_update(de_manager *const);
unsigned de_manager_delete(de_manager *const, void *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);

#define de_manager_init(MANAGER, BYTES) \
    de_manager_initEx(MANAGER, sizeof(de_entity) + BYTES, &de_entity_update, &de_entity_destroy);
