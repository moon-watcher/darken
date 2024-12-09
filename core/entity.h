#pragma once

#include "state.h"

typedef struct de_entity
{
    de_state_f state;
    de_state_f destructor;
    //
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_state de_entity_exec(de_entity *const);
void de_entity_delete(de_entity *const);
