#pragma once

typedef struct deDefinition_t
{
    // Common
    char *name;

    // Entity
    deManager_t *manager;
    deState_t *state; // constructor | destructor 

    // Manager
    unsigned maxEntities;
    unsigned maxBytes;

    // System
    void (*updateFn)();
    unsigned maxItems;
    unsigned params;
} deDefinition_t;
