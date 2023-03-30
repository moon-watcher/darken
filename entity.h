#pragma once

typedef struct deManager_t deManager_t;
typedef struct deEntity_t  deEntity_t;

typedef struct deState_t
{
    void (*enter)  ( deEntity_t *const );
    void (*update) ( deEntity_t *const );
    void (*leave)  ( deEntity_t *const );
} deState_t;

typedef struct deEntity_t
{
    void (*update)(struct deEntity_t *const);
    
    deState_t *state;
    deState_t *xtor; // constructor | destructor
    deManager_t *manager;
    
    // Index of entities in deManager_t's entities array
    unsigned index;

    // Bytes for casting data & components
    unsigned char data[];
} deEntity_t;

deEntity_t *deEntity_new    ( deState_t *const, deManager_t *const );
void        deEntity_delete ( deEntity_t * );
void        deEntity_change ( deEntity_t *const, const deState_t *const );
void        deEntity_force  ( deEntity_t *const, const deState_t *const );
