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

void de_manager_init(deManager_t *const, const deDefinition_t *);
void de_manager_end(deManager_t *const);
void de_manager_update(deManager_t *const);