#pragma once

typedef struct deDefinition_t
{
    // Common
    char *name;

    // Entity
    void (*constructor)(deEntity_t *const);
    void (*destructor)(deEntity_t *const);
    deManager_t *manager;
    const deState_t *state;

    // Manager
    unsigned maxEntities;
    unsigned maxBytes;

    // System
    void (*updateFn)();
    unsigned maxItems;
    unsigned params;
} deDefinition_t;
