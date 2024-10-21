#pragma once

#include "../libs/uclist/uclist.h"

typedef struct de_manager
{
    uclist list;
    void *data;
} de_manager;

void de_manager_init(de_manager *const, unsigned, unsigned);
void de_manager_update(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);

//

typedef struct de_entity de_entity;

de_entity *de_manager_new(de_manager *const, void (*)());
void de_manager_delete(de_manager *const, de_entity *const);