#pragma once

#include <stdint.h>
#include "state.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
    de_state state;
    de_state destructor;
    de_manager *manager;
    uint16_t index;
    uint8_t data[];
} de_entity;

void de_entity_state(de_entity *, de_state);
void de_entity_pause(de_entity *);
void de_entity_resume(de_entity *);
void de_entity_delete(de_entity *);
uint16_t de_entity_isPaused(de_entity *);
uint16_t de_entity_isActive(de_entity *);
uint16_t de_entity_isDeleted(de_entity *);
void de_entity_swapIndex(de_entity *, uint16_t);
