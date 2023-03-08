#pragma once

typedef struct deDefinition_t
{
    // Common
    char *name;

    // entity
    void (*constructor)(deEntity_t *const);
    void (*destructor) (deEntity_t *const);
    deManager_t *manager;
    const deState_t *initialState;

    // manager
    unsigned maxEntities;
    unsigned maxBytesEntities;

    // system
    void (*updateSystem)();
    unsigned maxVarsSystem;
} deDefinition_t;
