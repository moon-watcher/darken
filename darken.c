#include "darken.h"
#include "config.h"
#include "common/include.h"

static int loop;

void darken_loop(unsigned size, de_state *const state)
{
    loop = 1;
    size += sizeof(de_entity);
    de_entity *entity = malloc(size);
    memset(entity, 0, size);

    entity->delay = 0;
    entity->state = state;
    entity->newState = 0;

    de_state_enter(entity);

    while (loop == 1)
        de_noAPI_entity_update(entity);

    de_state_leave(entity);

    free(entity);
}

void darken_break()
{
    loop = 0;
}