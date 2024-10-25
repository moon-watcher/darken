#include "config.h"
#include "core/entity.h"

static bool loop;

void darken_loop(void (*state[static 1])(), unsigned size)
{
    loop = true;

    de_entity *entity = malloc(sizeof(de_entity) + size);
    de_entity_set(entity, state);

    while (loop)
    {
        entity->state(entity, entity->data);
    }

    free(entity);
}

void darken_break()
{
    loop = false;
}
