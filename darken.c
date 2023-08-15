#include "darken.h"

static int exit;

int darken(const de_state *const state)
{
    exit = 0;
    int *const exitCode = &exit;

    de_entity *const entity = de_entity_new(state);
    de_state_f const update = state->update;

    while (0 == *exitCode)
        update(entity);

    de_entity_delete(entity);

    return *exitCode;
}

void darken_end(int exitCode)
{
    exit = exitCode;
}