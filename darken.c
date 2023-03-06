#include "darken.h"

static int exitCode = 0;

int darken(const State *initialState)
{
    exitCode = 0;
    int *x = &exitCode;

    Manager manager;
    manager_init(&manager, &(Managerdef){.name = "asdf", .maxEntities = 1});
    Entity *const e = entity_new(&(Entitydef){.manager = &manager, .initialState = initialState });

    while (!*x)
        e->state->update(e);

    manager_destroy(&manager);

    return exitCode;
}

void darken_end(int code)
{
    exitCode = code;
}