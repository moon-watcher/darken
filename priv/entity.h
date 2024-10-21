#pragma once

#include "../core/state.h"
#include "../core/manager.h"

enum
{
    STATUS_UPDATE,
    STATUS_DELETE,
    STATUS_SET
};

#define _EXEC(METHOD, ENTITY)                        \
    if (ENTITY->state->METHOD != 0)                  \
    {                                                \
        ENTITY->state->METHOD(ENTITY, ENTITY->data); \
    }

void _de_entity_init(de_entity *const, de_manager *const, void (*)());
void _de_entity_update(de_entity *const);
void _de_entity_destroy(de_entity *const);
