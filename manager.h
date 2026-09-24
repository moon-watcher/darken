#pragma once

#include "entity.h"
#include "../glibs/uclist.h"

typedef struct de_manager
{
    uclist list;
    uint16_t pause_index;
    //
    char *name;
} de_manager;

void de_manager_init(de_manager *, uint16_t, uint16_t);
de_entity *de_manager_new(de_manager *);
void de_manager_update(de_manager *);
void de_manager_pause(de_manager *);
void de_manager_resume(de_manager *);
void de_manager_reset(de_manager *);
void de_manager_end(de_manager *);

/**
 * @brief de_manager_iterate[All] Iterates over the entities of a manager in reverse order.
 *
 * Variables available inside CODE:
 *
 * @var ENTITIES de_entity**   Pointer to the manager's entity array.
 * @var INDEX    uint16_t      Current index in the array. Can be used for direct access: ENTITIES[INDEX].
 * @var ENTITY   de_entity*    Pointer to the current entity.
 *
 * @note de_manager_iterate    iterates only over active entities (above pause_index).
 * @note de_manager_iterateAll iterates over all entities (active + paused).
 * @note Do not modify ENTITIES or INDEX inside CODE — undefined behavior.
 * @note Do not use _de_manager_iterate directly.
 */
#define de_manager_iterate(MANAGER, CODE) _de_manager_iterate(MANAGER, (MANAGER)->pause_index, CODE)
#define de_manager_iterateAll(MANAGER, CODE) _de_manager_iterate(MANAGER, 0, CODE)

#define _de_manager_iterate(MANAGER, LIMIT, CODE)     \
    do                                                \
    {                                                 \
        uint16_t INDEX = (MANAGER)->list.size;        \
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

/**
 * @brief de_manager_apply[All] Collects matching entities then applies an action to each.
 *
 * Safe to use with de_entity_delete, de_entity_pause and de_entity_resume —
 * entities are collected before any action is applied, avoiding index corruption.
 *
 * @param MANAGER  de_manager*   The manager to operate on.
 * @param FILTER   expression    Boolean expression evaluated for each entity. Use 1 to match all entities.
 * @param ACTION   function      Function applied to each matched entity: void f(de_entity *).
 *
 * Variables available inside FILTER:
 *
 * @var ENTITIES de_entity**   Pointer to the manager's entity array.
 * @var INDEX    uint16_t      Current index in the array.
 * @var ENTITY   de_entity*    Pointer to the current entity being evaluated.
 *
 * @note de_manager_apply    operates only on active entities (above pause_index).
 * @note de_manager_applyAll operates on all entities (active + paused).
 * @note Maximum of 32 entities can be collected per call.
 * @note Do not modify ENTITIES or INDEX inside FILTER — undefined behavior.
 * @note Do not use _de_manager_apply directly.
 *
 * @example
 *   de_manager_applyAll(&g_manager, ENTITY->state == s_enemy_walk, de_entity_pause);
 *   de_manager_apply   (&g_manager, 1,                             de_entity_delete);
 */
#define de_manager_apply(MANAGER, FILTER, ACTION) _de_manager_apply(de_manager_iterate, MANAGER, FILTER, ACTION)
#define de_manager_applyAll(MANAGER, FILTER, ACTION) _de_manager_apply(de_manager_iterateAll, MANAGER, FILTER, ACTION)

#define _de_manager_apply(ITERATE, MANAGER, FILTER, ACTION)             \
    do                                                                  \
    {                                                                   \
        uint16_t count = 0;                                             \
        de_entity *targets[(MANAGER)->list.size + 1];                   \
                                                                        \
        ITERATE(MANAGER, { (FILTER) && (targets[count++] = ENTITY); }); \
                                                                        \
        while (count--)                                                 \
            (ACTION)(targets[count]);                                   \
                                                                        \
    } while (0)
