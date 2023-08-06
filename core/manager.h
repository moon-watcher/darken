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

    int pause;
} deManager_t;

void deManager_init    ( deManager_t *const manager, unsigned maxEntities, unsigned maxBytes );
void deManager_end     ( deManager_t *const manager );
void deManager_reset   ( deManager_t *const manager );
void deManager_update  ( deManager_t *const manager );
void deManager_timeout ( deManager_t *const manager, unsigned timeout );
void deManager_pause   ( deManager_t *const manager );
void deManager_resume  ( deManager_t *const manager );

deEntity_t *deManager_newEntity ( deManager_t *const manager, const deState_t *const state );
deEntity_t *deManager_getEntity ( deManager_t *const manager, unsigned index );