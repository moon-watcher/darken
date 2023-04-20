#pragma once

typedef struct deManager_t deManager_t;
typedef struct deState_t   deState_t;

typedef struct deEntity_t
{
    deState_f *updateFn;

    deState_t *state;
    deState_t *xtor;       // constructor, entity executor, destructor
    
    deManager_t *manager;
    unsigned index;        // Index of entities in deManager_t's entities array
    unsigned char data[];  // Bytes for casting data & components
} deEntity_t;

deEntity_t *deEntity_newLess ( const deState_t * );
deEntity_t *deEntity_new     ( const deState_t *, deManager_t *const );
void        deEntity_update  ( deEntity_t *const );
void        deEntity_change  ( deEntity_t *const, const deState_t *const );
void        deEntity_set     ( deEntity_t *const, const deState_t *const );
void        deEntity_delete  ( deEntity_t *const );