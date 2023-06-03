#pragma once

typedef struct deEntity_t deEntity_t;

typedef struct deManager_t
{
    deEntity_t **entityList;

    // Next free position in entities array
    unsigned int free_pos;

    // Number of allocated entities
    unsigned int allocated_entities;

    unsigned int maxEntities;
    unsigned int maxBytes;

    int pause;
} deManager_t;

void deManager_init    ( deManager_t *const, unsigned int maxEntities, unsigned int maxBytes );
void deManager_end     ( deManager_t *const );
void deManager_reset   ( deManager_t *const );
void deManager_update  ( deManager_t *const );
void deManager_timeout ( deManager_t *const, unsigned int );
void deManager_pause   ( deManager_t *const );
void deManager_resume  ( deManager_t *const );

deEntity_t *deManager_newEntity ( deManager_t *const, const deState_t *const );
deEntity_t *deManager_getEntity ( deManager_t *const, unsigned int );