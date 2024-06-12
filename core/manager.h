#pragma once

#include "../libs/uplist.h"
#include "entity.h"

typedef uplist de_manager;

void de_manager_init(de_manager *const, unsigned);
de_entity *de_manager_createEntity(de_manager *const, de_state *);
int de_manager_deleteEntity(de_manager *const, de_entity *);
void de_manager_update(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);