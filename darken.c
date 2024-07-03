#include "darken.h"
#include "config.h"

void darken_loop(unsigned size, de_state *const state)
{
    de_entity *entity = malloc(sizeof(de_entity) + size);
    entity->newState = entity->state = state;

    while (entity)
    {
        de_state_enter(entity);

        while (entity->newState == entity->state)
            de_state_update(entity);

        de_state_leave(entity);
    }
}