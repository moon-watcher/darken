#pragma once

#include "state.h"

typedef struct de_entity
{
    de_state_f state;
    de_state_f destructor;

    // Bytes for components
    unsigned char data[];
} de_entity;

de_state de_entity_exec(de_entity *const);
void de_entity_delete(de_entity *const);
void de_entity_state(de_entity *const, de_state_f);
void de_entity_destructor(de_entity *const, de_state_f);
