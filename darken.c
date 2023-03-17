#include "darken.h"

static int _exit = 0;

int darken(deDefinition_t *ed)
{
    const deDefinition_t manager_def = { .maxEntities = 1 };
    
    _exit = 0;
    int *exitCode = &_exit;
    
    deManager_t manager;
    ed->manager = &manager;

    deManager_init(ed->manager, &manager_def);
    deEntity_t *const e = deEntity_new(ed);

    while (!*exitCode)
        e->state->update(e);

    return *exitCode;
}

void darken_end(int exitCode)
{
    _exit = exitCode;
}