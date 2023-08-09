#include "darken.h"

static int exit;

int darken(const deState_t *const s)
{
    exit = 0;
    int *exitCode = &exit;

    deEntity_t *const e = deEntity_new(s);
    deState_f const update = e->updateFn;

    while (!*exitCode)
        update(e);

    deEntity_delete(e);

    return *exitCode;
}

void darken_end(int exitCode)
{
    exit = exitCode;
}