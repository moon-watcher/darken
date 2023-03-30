#pragma once

typedef struct deEntity_t deEntity_t;

typedef struct deManager_t
{
    deEntity_t **entityList;

    // Next free position in entities array
    unsigned free_pos;

    // Number of allocated entities
    unsigned allocated_entities;

    unsigned maxEntities;
    unsigned maxBytes;
} deManager_t;

void deManager_init   ( deManager_t *const, unsigned, unsigned );
void deManager_end    ( deManager_t *const );
void deManager_update ( deManager_t *const );