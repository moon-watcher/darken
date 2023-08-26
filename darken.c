#include "darken.h"
#include "include.h"

static de_manager man;

void darken_init(darken *const de, const de_state *const s)
{
    de->state = s;
    de->entity = de_entity_new(de->state);

    de_state_f const enter = de->state->enter;

    if (enter != 0)
        enter(de->entity);
}

void darken_update(darken *const de)
{
    de_state_f const update = de->state->update;
    de_entity *const entity = de->entity;

    update(entity);
}

void darken_state(darken *const de, const de_state *const s)
{
    de_state_set(de->entity, s);
}

void darken_loop(darken *const de)
{
    de->loop = 1;

    char *const loop = &de->loop;
    de_state_f const update = de->state->update;
    de_entity *const entity = de->entity;

    while (loop)
        update(entity);
}

void darken_end(darken *const de)
{
    de_entity_delete(de->entity);
    de->entity = 0;
    de->state = 0;
    de->loop = 0;
}