#pragma once

#include "state.h"
#include "manager.h"

typedef struct de_entity
{
    de_state *(*update)();
    de_state *(*destructor)();
    de_state *state;
    de_manager *manager;
    int status;
    unsigned timer;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

#define _EXEC(METHOD, ENTITY)                        \
    if (ENTITY->state->METHOD != 0)                  \
    {                                                \
        ENTITY->state->METHOD(ENTITY, ENTITY->data); \
    }

void de_entity_set(de_entity *const, de_state *);
void de_entity_delete(de_entity *const);

// Darken private
void _de_entity_init(de_entity *const, de_manager *const, void (*)());
void _de_entity_update(de_entity *const);
void _de_entity_destroy(de_entity *const);
