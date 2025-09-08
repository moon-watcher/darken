#pragma once

#include "state.h"

typedef struct de_entity
{
    de_state state;
    de_state destructor;

    unsigned char data[]; // Bytes for data
} de_entity;

#define de_entity_set(ENTITY, STATE) ({ ENTITY->state = STATE; })
#define de_entity_destructor(ENTITY, STATE) ({ ENTITY->destructor = STATE; })
#define de_entity_delete(ENTITY) ({ ENTITY->state = 0; })
#define de_entity_update(ENTITY) ({ ENTITY->state = ENTITY->state(ENTITY->data); })
