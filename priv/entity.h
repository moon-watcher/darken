#pragma once

#define _EXEC(METHOD, ENTITY)                        \
    if (ENTITY->state->METHOD != 0)                  \
    {                                                \
        ENTITY->state->METHOD(ENTITY, ENTITY->data); \
    }

typedef struct de_entity de_entity;
typedef struct de_manager de_manager;

void _de_entity_init(de_entity *const, de_manager *const, void (*)());
void _de_entity_update(de_entity *const);
void _de_entity_destroy(de_entity *const);
