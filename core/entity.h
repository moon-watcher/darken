#pragma once

#include <stdint.h>
#include "state.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
    de_state state;
    de_state destructor;
    de_manager *manager;
    uint8_t data[];
} de_entity;

void de_entity_delete(de_entity *);

#define DE_ENTITY_STATE(ENTITY, STATE) ((ENTITY)->state = (STATE))
#define DE_ENTITY_DELETE(ENTITY) DE_ENTITY_STATE((ENTITY), DE_STATE_DELETE)

#define DE_ENTITY_DELETENOW(ENTITY)                    \
    uclist_remove(&(ENTITY)->manager->list, (ENTITY)); \
    (ENTITY)->destructor && (ENTITY)->destructor((ENTITY)->data);
