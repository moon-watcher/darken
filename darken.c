#include "darken.h"

static int exitCode = 0;

int darken(const deState_t *initialState, void (*constructor)(deEntity_t *const))
{
    exitCode = 0;
    int *x = &exitCode;

    deManager_t manager;
    const deDefinition_t ed = {.name = "Darken main entity", .manager = &manager, .initialState = initialState, .constructor = constructor};
    const deDefinition_t md = {.name = "Darken main manager", .maxEntities = 1};

    deManager_init(&manager, &md);
    deEntity_t *const e = deEntity_new(&ed);

    while (!*x)
        e->state->update(e);

    deManager_end(&manager);

    return exitCode;
}

void darken_end(int code)
{
    exitCode = code;
}