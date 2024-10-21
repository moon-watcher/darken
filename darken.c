#include "darken.h"
#include "config.h"
#include "priv/entity.h"

static bool loop;

void darken_loop(de_state *const state, unsigned size)
{
    loop  = true;
    de_entity *entity = malloc(sizeof(de_entity) + size);

    entity->state = state;
    entity->handle = state->Handle;
    entity->event = 0;
    entity->timer = 0;

    _EXEC(Enter, entity);

    while (loop)
    {
        _de_entity_update(entity);
    }

    _EXEC(Exit, entity);

    free(entity);
}

void darken_break()
{
    loop = false;
}