#pragma once

typedef struct deEntity_t
{
    void (*update)(struct deEntity_t *const);
    
    const deState_t *state;
    deDefinition_t *definition;
    
    // Index of this entity in the deManager_t's entities array
    unsigned index;

    // Bytes for casting data & components
    unsigned char data[];
} deEntity_t;

deEntity_t *deEntity_new       (const deDefinition_t *);
void        deEntity_delete    (deEntity_t *);
void        deEntity_set_state (deEntity_t *const, const deState_t *const);