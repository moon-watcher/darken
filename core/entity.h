#pragma once

#include "state.h"

typedef struct de_entity
{
    de_state state;
    de_state destructor;

    unsigned char data[]; // Bytes for data
} de_entity;
