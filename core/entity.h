#pragma once

typedef struct deManager_t deManager_t;
typedef struct deState_t   deState_t;

typedef struct deEntity_t
{
    deState_t state;
    deState_f destructor;
    // deState_t *xtor;       // constructor, entity executor, destructor
    
    deManager_t *manager;
    unsigned char data[];  // Bytes for casting data & components
} deEntity_t;


deEntity_t *deEntity_new    ( const deState_t *const entity );
void        deEntity_delete ( deEntity_t *const entity );

void deState_set      ( deEntity_t *const entity, const deState_t *const state );
void deState_force    ( deEntity_t *const entity, const deState_t *const state );
void deState_enter    ( deEntity_t *const entity );
void deState_update   ( deEntity_t *const entity );
void deState_leave    ( deEntity_t *const entity );
void deState_destruct ( deEntity_t *const entity );