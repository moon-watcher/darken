#pragma once

#include "../core/entity.h"

void de_NOAPI_entity_destroy(de_entity *const);
void de_NOAPI_entity_update(de_entity *const);

#define de_NOAPI_state_enter(ENTITY)                     \
    if (ENTITY->state != 0 && ENTITY->state->enter != 0) \
    {                                                    \
        ENTITY->state->enter(ENTITY, ENTITY->data);      \
    }

#define de_NOAPI_state_update(ENTITY)                     \
    if (ENTITY->state != 0 && ENTITY->state->update != 0) \
    {                                                     \
        ENTITY->state->update(ENTITY, ENTITY->data);      \
    }

#define de_NOAPI_state_leave(ENTITY)                     \
    if (ENTITY->state != 0 && ENTITY->state->leave != 0) \
    {                                                    \
        ENTITY->state->leave(ENTITY, ENTITY->data);      \
    }
