#pragma once

#include "state.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
    de_state state;
    de_state destructor;
    de_manager *manager;

    unsigned char data[]; // Bytes for data
} de_entity;

de_entity *de_entity_set(de_entity *const, de_state);
de_entity *de_entity_destructor(de_entity *const, de_state);
de_entity *de_entity_delete(de_entity *const);
de_entity *de_entity_pause(de_entity *const);
de_entity *de_entity_update(de_entity *const);
