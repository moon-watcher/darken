#include "darken.h"

static int exitCode = 0;

int darken(const deState_t *initialdeState_t)
{
    exitCode = 0;
    int *x = &exitCode;

    deManager_t manager;
    const deDefinition_t ed = {.name = "Darken main entity", .manager = &manager, .initialdeState_t = initialdeState_t};
    const deDefinition_t md = {.name = "Darken main manager", .maxEntities = 1};

    de_manager_init(&manager, &md);
    deEntity_t *const e = de_entity_new(&ed);

    while (!*x)
        e->state->update(e);

    de_manager_end(&manager);

    return exitCode;
}

void darken_end(int code)
{
    exitCode = code;
}