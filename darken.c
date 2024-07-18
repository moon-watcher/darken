#include "darken.h"
#include "config.h"
#include "core/NOAPI.h"

static int loop;

void darken_loop(unsigned size, de_state *state)
{
    loop = 1;
    size += sizeof(de_entity);
    de_entity *entity = malloc(size);
    memset(entity, 0, size);

    state = state ?: &de_state_empty;
    entity->update = state->update ?: de_state_nullf;
    entity->leave = state->leave ?: de_state_nullf;

    if (state->enter != 0)
        state->enter(entity, entity->data);

    while (loop == 1)
        NOAPI_entity_array[entity->ctrl](entity);

    entity->leave(entity, entity->data);

    free(entity);
}

void darken_break()
{
    loop = 0;
}