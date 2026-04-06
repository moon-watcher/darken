#pragma once

#include "entity.h"
// #include "../uclist/uch"

#include <genesis.h>

#define ALIGN2(x) (((x)+1)&~1)
#define DE_ENTITY_SIZE(BYTES) ALIGN2(sizeof(de_entity) + (BYTES))

typedef struct
{
    uint16_t itemSize;
    uint16_t capacity;
    void *list;
} de_manager_def;

#define DEFIN(SIZE, CAP) \
    { .itemSize = DE_ENTITY_SIZE(SIZE), .capacity = CAP, .list = malloc(DE_ENTITY_SIZE(SIZE)*CAP) }

typedef struct de_manager
{
    de_manager_def;  // itemSize, capacity, list
    uint16_t size;
    uint16_t pause_index;
} de_manager;

//void de_manager_init(de_manager *, uint16_t);
void de_manager_init(de_manager *, de_manager_def *);
de_entity *de_manager_new(de_manager *);
void de_manager_update(de_manager *);
void de_manager_pause(de_manager *);
void de_manager_resume(de_manager *);
void de_manager_reset(de_manager *);
void de_manager_end(de_manager *);

/**
 * @brief de_manager_iterate[all] Iterates over the entities of a manager in reverse order.
 *
 * Variables available inside CODE:
 *
 * @var ENTITIES de_entity**   Pointer to the manager's entity array.
 * @var INDEX    uint16_t      Current index in the array. Can be used for direct access: ENTITIES[INDEX].
 * @var ENTITY   de_entity*    Pointer to the current entity.
 *
 * @note Do not modify ENTITIES or INDEX inside CODE — undefined behavior.
 * @note Do not use _de_manager_iterate directly.
 */
#define de_manager_iterate(MANAGER, CODE) _de_manager_iterate(MANAGER, (MANAGER)->pause_index, CODE)
#define de_manager_iterateAll(MANAGER, CODE) _de_manager_iterate(MANAGER, 0, CODE)

#define _de_manager_iterate(MANAGER, LIMIT, CODE)           \
    do {                                                   \
        uint16_t INDEX = (MANAGER)->size;                 \
        if (INDEX <= LIMIT) break;                         \
        while (INDEX-- > LIMIT) {                          \
            de_entity *ENTITY = getEntity(MANAGER, INDEX); \
            CODE;                                         \
        }                                                  \
    } while (0)
