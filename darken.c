#include "darken.h"

static int exitCode = 0;

int darken(const State *initialState)
{
    exitCode = 0;
    int *x = &exitCode;

    Manager manager;
    const Entitydef ed = {.manager = &manager, .initialState = initialState};
    const Managerdef md = {.maxEntities = 1};

    manager_init(&manager, &md);    
    Entity *const e = entity_new(&ed);

    while (!*x)
        e->state->update(e);

    manager_destroy(&manager);

    return exitCode;
}

void darken_end(int code)
{
    exitCode = code;
}