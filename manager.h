#pragma once

typedef struct deManager_t
{
    deEntity_t **entityList;

    // Next free position in entities array
    unsigned free_pos;

    // Number of allocated entities
    unsigned allocated_entities;

    // Definition
    const deDefinition_t *definition;
} deManager_t;

void deManager_init    (deManager_t *const, const deDefinition_t *);
void deManager_end     (deManager_t *const);
void deManager_update  (deManager_t *const);
void deManager_iterate (deManager_t *const, deIterator_t);

#define deManager_foreach(M, E) \
    deEntity_t *E = NULL; \
    for (unsigned i = 0, E = M->entityList[0]; E = M->entityList[i], i < M->free_pos; i++)
