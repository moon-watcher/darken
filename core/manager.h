#pragma once

#include "../libs/uplist.h"

typedef struct de_entity de_entity;

typedef struct de_manager
{
    uplist list;
    void *data;
} de_manager;

void de_manager_init(de_manager *const, unsigned);
de_entity *de_manager_new(de_manager *const);
void de_manager_update(de_manager *const);
unsigned de_manager_delete(de_manager *const, de_entity *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);