#pragma once

#include "../libs/uplist.h"

typedef struct de_manager
{
    uplist list;
    void (*updateItem)();
    void (*destroyItem)();
} de_manager;

void de_manager_init(de_manager *const, unsigned);
void *de_manager_new(de_manager *const);
void de_manager_update(de_manager *const);
unsigned de_manager_delete(de_manager *const, void *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);