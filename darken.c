#include "darken.h"

void darken_init(darken *const de, const de_state *const s)
{
    de->exitCode = 0;
    de->entity = de_entity_new(s);
    de->state = s;
}

void darken_update(darken *const de)
{
    de_state_f const update = de->state->update;
    de_entity *const entity = de->entity;

    update(entity);
}

void darken_state(darken *const de, const de_state *const s)
{
    de->state = s;
}

void darken_loop(darken *const de)
{
    int *const exitCode = &de->exitCode;
    de_state *const state = de->state;
    de_entity *const entity = de->entity;

    while (0 != exitCode)
        state->update(entity);
}

void darken_end(darken *const de)
{
    de->exitCode = 1;
    de_entity_delete(de->entity);
    de->entity = 0;
}

// void darken_loop(darken *const de)
// {
//     int *const exitCode = &de->exitCode;
//     de_state_f const update = de->state->update;
//     de_entity *const entity = de->entity;

//     while (0 != exitCode)
//         update(entity);
// }