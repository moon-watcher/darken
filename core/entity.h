#pragma once

#include "state.h"

typedef struct de_entity
{
    de_state_f state;
    de_state_f destructor;

    unsigned char data[]; // Bytes for data
} de_entity;

de_entity *de_entity_state(de_entity *const, de_state_f);
de_entity *de_entity_destructor(de_entity *const, de_state_f);
de_entity *de_entity_delete(de_entity *const);
