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
#define de_manager_iterateall(MANAGER, CODE) _de_manager_iterate(MANAGER, 0, CODE)

#define _de_manager_iterate(MANAGER, LIMIT, CODE)     \
    do                                                \
    {                                                 \
        uint16_t INDEX = (MANAGER)->list.size;        \
                                                      \
        if (INDEX <= LIMIT)                           \
            break;                                    \
                                                      \
        de_entity **ENTITIES = (MANAGER)->list.items; \
        while (INDEX-- > LIMIT)                       \
        {                                             \
            de_entity *ENTITY = ENTITIES[INDEX];      \
            CODE;                                     \
        }                                             \
    } while (0)
