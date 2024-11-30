#pragma once

#include "state.h"

typedef struct de_entity
{
    de_state (*handler)();
    de_state (*destructor)();
    //
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_state de_entity_update(de_entity *const);
de_state de_entity_delete(de_entity *const);
