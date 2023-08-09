#pragma once

typedef struct deEntity_t deEntity_t;

typedef struct deManager_t
{
    deEntity_t **entityList;

    // Next free position in entities array
    unsigned int freePos;

    // Number of allocated entities
    unsigned int allocatedEntities;

    unsigned int maxEntities;
    unsigned int maxBytes;

    int pause;
} deManager_t;

void deManager_init    ( deManager_t *const manager, unsigned int maxEntities, unsigned int maxBytes );
void deManager_end     ( deManager_t *const manager );
void deManager_reset   ( deManager_t *const manager );
void deManager_update  ( deManager_t *const manager );
void deManager_timeout ( deManager_t *const manager, unsigned int timeout );
void deManager_pause   ( deManager_t *const manager );
void deManager_resume  ( deManager_t *const manager );

deEntity_t *deManager_createEntity     ( deManager_t *const manager, const deState_t *const state );
deEntity_t *deManager_getEntityByIndex ( deManager_t *const manager, unsigned int index );
void        deManager_deleteEntity     ( deManager_t *const manager, deEntity_t *const entity );