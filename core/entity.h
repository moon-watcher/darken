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

#define DE_ENTITY_IS_PAUSED(ENTITY) ((ENTITY)->index < (ENTITY)->manager->pause_index)
#define DE_ENTITY_IS_ACTIVE(ENTITY) ((ENTITY)->index >= (ENTITY)->manager->pause_index && (ENTITY)->index < (ENTITY)->manager->list.size)
#define DE_ENTITY_IS_DELETED(ENTITY) ((ENTITY)->index >= (ENTITY)->manager->list.size)

#define DE_ENTITY_SET(ENTITY, STATE) ((ENTITY)->state = (STATE))
#define DE_ENTITY_EXEC(ENTITY, STATE) ((STATE)((ENTITY)->data))
#define DE_ENTITY_UPDATE(ENTITY, STATE) (!DE_STATE_IS_LOOP((STATE) = DE_ENTITY_EXEC((ENTITY), (STATE))) && ((ENTITY)->state = (STATE)))
