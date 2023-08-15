#pragma once

typedef struct deManager_t deManager_t;
typedef struct deState_t   deState_t;

typedef struct deEntity_t
{
    deState_t *state;
    deState_f destructor;
    // deState_t *xtor;       // constructor, entity executor, destructor
    
    deManager_t *manager;
    unsigned char data[];  // Bytes for casting data & components
} deEntity_t;


deEntity_t *deEntity_new    ( const deState_t *const entity );
void        deEntity_delete ( deEntity_t *const entity );

void deEntity_stateSet      ( deEntity_t *const entity, const deState_t *const state );
void deEntity_stateForce    ( deEntity_t *const entity, const deState_t *const state );
void deEntity_stateEnter    ( deEntity_t *const entity );
void deEntity_stateUpdate   ( deEntity_t *const entity );
void deEntity_stateLeave    ( deEntity_t *const entity );
void deEntity_stateDestruct ( deEntity_t *const entity );