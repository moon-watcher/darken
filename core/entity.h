#pragma once

typedef struct deManager_t deManager_t;
typedef struct deState_t   deState_t;

typedef struct deEntity_t
{
    deState_f updateFn;

    deState_t *state;
    deState_t *xtor;       // constructor, entity executor, destructor
    
    deManager_t *manager;
    unsigned char data[];  // Bytes for casting data & components
} deEntity_t;

deEntity_t *deEntity_new        ( const deState_t *const entity );
void        deEntity_setState   ( deEntity_t *const entity, const deState_t *const state );
void        deEntity_forceState ( deEntity_t *const entity, const deState_t *const state );
void        deEntity_delete     ( deEntity_t *const entity );


#define deEntity_update(E) \
    DARKEN_STATE_EXEC(E, E->updateFn)
