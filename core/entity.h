#pragma once

#include "state.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
    de_state handler;
    de_state destructor;
    de_manager *manager;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

int de_entity_delete(de_entity *const);
void de_entity_destructor(de_entity *const, de_state);
void de_entity_set(de_entity *const, de_state);
void de_entity_exec(de_entity *const, de_state);
