#include "darken.h"

static int exit;

int darken(const deState_t *const state)
{
    exit = 0;
    int *const exitCode = &exit;

    deEntity_t *const entity = deEntity_new(state);
    deState_f const update = state->update;

    while (0 == *exitCode)
        update(entity);

    deEntity_delete(entity);

    return *exitCode;
}

void darken_end(int exitCode)
{
    exit = exitCode;
}