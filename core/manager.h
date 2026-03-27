#pragma once

#include <stdint.h>
#include "entity.h"
#include "../uclist/uclist.h"

typedef struct de_manager
{
    uclist list;
    uint16_t pause_index;
} de_manager;

void de_manager_init(de_manager *, uint16_t);
de_entity *de_manager_new(de_manager *);
void de_manager_update(de_manager *);
void de_manager_pause(de_manager *);
void de_manager_resume(de_manager *);
de_entity **de_manager_getEntities(de_manager *);
uint16_t de_manager_getCapacity(de_manager *);
uint16_t de_manager_countAll(de_manager *);
uint16_t de_manager_count(de_manager *);
void de_manager_reset(de_manager *);
void de_manager_end(de_manager *);

//

#define DE_MANAGER_ITERATE(MANAGER, CODE) \
    _DE_MANAGER_ITERATE(MANAGER, (MANAGER)->pause_index, CODE)

#define DE_MANAGER_ITERATEALL(MANAGER, CODE) \
    _DE_MANAGER_ITERATE(MANAGER, 0, CODE)

#define _DE_MANAGER_ITERATE(MANAGER, LIMIT, CODE)               \
    {                                                           \
        de_entity **ENTITIES = de_manager_getEntities(MANAGER); \
        uint16_t INDEX = de_manager_countAll(MANAGER);          \
                                                                \
        while (INDEX-- > LIMIT)                                 \
        {                                                       \
            de_entity *ENTITY = ENTITIES[INDEX];                \
            void *DATA = ENTITY->data;                          \
            de_state STATE = ENTITY->state;                     \
                                                                \
            CODE;                                               \
        }                                                       \
    }
