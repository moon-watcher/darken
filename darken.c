#include "darken.h"

static int _exit = 0;

static const deDefinition_t _manager_def = {
    .maxEntities      = 1,
    .maxBytesEntities = 0,
};

int darken(const deDefinition_t *ed)
{
    _exit = 0;
    int *exit = &_exit;

    deManager_init(ed->manager, &_manager_def);
    deEntity_t *const e = deEntity_new(ed);

    while (!*exit)
        e->state->update(e);

    return *exit;
}

void darken_end(int exitCode)
{
    _exit = exitCode;
}