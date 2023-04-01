#pragma once

typedef struct deManager_t deManager_t;
typedef struct deState_t deState_t;

typedef struct deEntity_t
{
    deState_t *state;
    
    // construct | execute | destruct
    deState_t *xtor; 
    
    deManager_t *manager;
    
    // Index of entities in deManager_t's entities array
    unsigned index;

    // Bytes for casting data & components
    unsigned char data[];
} deEntity_t;

deEntity_t *deEntity_new    ( const deState_t *, deManager_t *const );
void        deEntity_update ( deEntity_t *const );
void        deEntity_delete ( deEntity_t * );
void        deEntity_change ( deEntity_t *const, const deState_t *const );
void        deEntity_jump   ( deEntity_t *const, const deState_t *const );
