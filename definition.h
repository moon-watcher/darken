#pragma once

typedef struct deDefinition_t
{
    // Common
    char *name;

    // deEntity_t
    void (*constructor)(deEntity_t *const);
    void (*destructor )(deEntity_t *const);
    deManager_t *manager;
    const deState_t *initialdeState_t;

    // deManager_t
    unsigned maxEntities;
    unsigned maxBytesEntities;

    // system
    void (*updatedeSystem_t)();
    unsigned maxVarsdeSystem_t;
} deDefinition_t;
