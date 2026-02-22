#pragma once

#include <stdint.h>
#include "config.h"

typedef struct
{
    uclist list;
    uint16_t pause_index;
    //
    // char *name;
} de_manager;

typedef void *(*de_state)(void *);

typedef struct
{
    de_state state;
    de_state destructor;
    de_manager *manager;
    uint8_t data[];
} de_entity;

void darken_init(de_manager *, uint16_t);
de_entity *darken_new(de_manager *);
void darken_update(de_manager *);
void darken_reset(de_manager *);
void darken_end(de_manager *);

#define DE_STATE_DELETE ((void *)0)
#define DE_STATE_LOOP ((void *)1)
#define DE_STATE_PAUSE ((void *)2)

#define DE_STATE_IS_ACTIVE(state) ((state) > (de_state)2)
#define DE_STATE_IS_DELETED(state) ((state) == (de_state)0)
#define DE_STATE_IS_PAUSED(state) ((state) == (de_state)2)
#define DE_STATE_NEED_UPDATE(state) ((state) != (de_state)1)

#define DE_ENTITY_SET_STATE(ENTITY, STATE) ((ENTITY)->state = (STATE))
#define DE_ENTITY_SET_DELETE(ENTITY) DE_ENTITY_SET_STATE((ENTITY), DE_STATE_DELETE)

#define DE_ENTITY_FORCE_DELETE(ENTITY)                 \
    uclist_remove(&(ENTITY)->manager->list, (ENTITY)); \
    (ENTITY)->destructor && (ENTITY)->destructor((ENTITY)->data);
