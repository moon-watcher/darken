#pragma once

#include "../libs/uplist.h"

typedef uplist de_manager;

#include "entity.h"

void de_manager_init(de_manager *const, unsigned);
de_entity *de_manager_new(de_manager *const, de_state_f);
void de_manager_update(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);