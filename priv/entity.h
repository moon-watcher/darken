#pragma once

typedef struct de_entity de_entity;
typedef struct de_manager de_manager;

#include "../core/state.h"

void _de_entity_init(de_entity *const, de_manager *const, de_state);
void _de_entity_update(de_entity *const);
void _de_entity_destroy(de_entity *const);
