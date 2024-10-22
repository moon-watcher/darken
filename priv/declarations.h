#pragma once

#include "../core/state.h"
#include "../core/entity.h"
#include "../core/manager.h"

void _entity_init(de_entity *const, de_manager *const, de_state);
void _entity_update(de_entity *const);
void _entity_destroy(de_entity *const);

void _manager_delete(de_manager *const, de_entity *const);
