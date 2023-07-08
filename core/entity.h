#pragma once

typedef struct deManager_t deManager_t;
typedef struct deState_t   deState_t;

typedef struct deEntity_t
{
    deState_f updateFn;

    deState_t *state;
    deState_t *xtor;       // constructor, entity executor, destructor
    
    deManager_t *manager;
    unsigned index;        // Index of entities in deManager_t's entities array
    unsigned char data[];  // Bytes for casting data & components
} deEntity_t;

deEntity_t *deEntity_newLess    ( const deState_t *const );
deEntity_t *deEntity_new        ( const deState_t *const, deManager_t *const );
void        deEntity_setState   ( deEntity_t *const, const deState_t *const );
void        deEntity_forceState ( deEntity_t *const, const deState_t *const );
void        deEntity_delete     ( deEntity_t *const );


#define deEntity_update(E) \
    deState_exec(E, E->updateFn)
    