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

void de_entity_update(de_entity *const entity);
int de_entity_delete(de_entity *const this);
void de_entity_destroy(de_entity *const entity);
void de_entity_setState(de_entity *const, de_state);
void de_entity_setManager(de_entity *const this, de_manager *const manager);
void de_entity_setDestructor(de_entity *const, de_state);
void de_entity_execState(de_entity *const, de_state);
