#include "darken.h"

static int _exit = 0;

int darken( /*deDefinition_t *ed*/ )
{
    _exit = 0;
    int *exitCode = &_exit;
    
    // deEntity_t *const e = deEntity_new(ed);

    // while (!*exitCode)
    //     e->update(e);

    return *exitCode;
}

void darken_end(int exitCode)
{
    _exit = exitCode;
}