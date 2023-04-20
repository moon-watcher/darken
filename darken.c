#include "darken.h"

static int exit = 0;

int darken(deState_t *const s)
{
    exit = 0;
    int *exitCode = &exit;

    deEntity_t *const e = deEntity_newLess(s); 

    while (!*exitCode)
        deEntity_update(e);

    deEntity_delete(e);

    return *exitCode;
}

void darken_end(int exitCode)
{
    exit = exitCode;
}