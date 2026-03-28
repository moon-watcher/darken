#pragma once

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
void de_manager_reset(de_manager *);
void de_manager_end(de_manager *);

#define DE_MANAGER_ITERATE(MANAGER, CODE) \
    _DE_MANAGER_ITERATE(MANAGER, (MANAGER)->pause_index, CODE)

#define DE_MANAGER_ITERATEALL(MANAGER, CODE) \
    _DE_MANAGER_ITERATE(MANAGER, 0, CODE)

#define _DE_MANAGER_ITERATE(MANAGER, LIMIT, CODE)     \
    do                                                \
    {                                                 \
        de_entity **ENTITIES = (MANAGER)->list.items; \
        uint16_t INDEX = (MANAGER)->list.size;        \
                                                      \
        while (INDEX-- > LIMIT)                       \
        {                                             \
            de_entity *ENTITY = ENTITIES[INDEX];      \
            void *DATA = ENTITY->data;                \
            de_state STATE = ENTITY->state;           \
                                                      \
            CODE;                                     \
        }                                             \
    } while (0)
