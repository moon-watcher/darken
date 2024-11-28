#pragma once

#include "../uclist/uclist.h"

typedef struct
{
    uclist list;
} de_manager;

void de_manager_init(de_manager *const, unsigned);
void de_manager_update(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);

#include "entity.h"

de_entity *de_manager_new(de_manager *const);
int de_manager_delete(de_manager *const, de_entity *const);
