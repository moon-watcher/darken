#include "darken.h"

static int _exit = 0;

int darken(const deDefinition_t *ed)
{
    _exit = 0;
    int *exit = &_exit;

    deManager_init(ed->manager, 0);
    deEntity_t *const e = deEntity_new(ed);

    while (!*exit)
        e->state->update(e);

    return *exit;
}

void darken_end(int exitCode)
{
    _exit = exitCode;
}