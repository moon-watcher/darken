#pragma once

#include "state.h"

typedef struct
{
    de_state handler;
    de_state destructor;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void de_entity_setState(de_entity *const, de_state);
void de_entity_setDestructor(de_entity *const, de_state);
