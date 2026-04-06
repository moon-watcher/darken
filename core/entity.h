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

void de_entity_pause(de_entity *);
void de_entity_resume(de_entity *);
void de_entity_delete(de_entity *);

#define de_entity_is_paused(ENTITY) ((ENTITY)->index < (ENTITY)->manager->pause_index)
#define de_entity_is_active(ENTITY) ((ENTITY)->index >= (ENTITY)->manager->pause_index && (ENTITY)->index < (ENTITY)->manager->size)
#define de_entity_is_deleted(ENTITY) ((ENTITY)->index >= (ENTITY)->manager->size)

#define de_entity_state(ENTITY, STATE) ((ENTITY)->state = (STATE))
#define de_entity_exec(ENTITY, STATE) ((STATE)((ENTITY)->data))
#define de_entity_update(ENTITY, STATE) (!de_state_is_loop((STATE) = de_entity_exec((ENTITY), (STATE))) && ((ENTITY)->state = (STATE)))
#define de_entity_destructor(ENTITY, STATE) ((ENTITY)->destructor = (STATE))

de_entity *getEntity(de_manager *mgr, uint16_t index);