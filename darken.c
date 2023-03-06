#include "darken.h"

static int exitCode = 0;

int darken(const State *initialState)
{
    Manager manager;
    manager_init(&manager, &(Managerdef){.maxEntities = 1});
    Entity *const e = entity_new(&(Entitydef){
        .manager = &manager,
        .initialState = initialState,
    });

    exitCode = 0;
    int *x = &exitCode;

    while (!*x)
        e->state->update(e);

    return exitCode;
}

void darken_end(int code)
{
    exitCode = code;
}