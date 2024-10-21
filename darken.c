#include "darken.h"
#include "config.h"

static unsigned loop;

void darken_loop(de_state *const state, unsigned size)
{
    loop = 1;
    de_entity *entity = malloc(sizeof(de_entity) + size);

    entity->state = state;
    entity->update = entity->state->update;
    entity->status = 0;
    entity->timer = 0;

    _EXEC(enter, entity);

    while (loop != 0)
    {
        de_entity_update(entity);
    }

    _EXEC(leave, entity);

    free(entity);
}

void darken_break()
{
    loop = 0;
}