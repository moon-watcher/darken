#include "config.h"
#include "core/entity.h"

static bool loop;

void darken_loop(de_state state, unsigned size)
{
    if (state == 0)
    {
        return;
    }

    de_entity *entity = malloc(sizeof(de_entity) + size);
    de_entity_setState(entity, state);

    loop = true;
    while (loop == true)
    {
        entity->handler(entity, entity->data);
    }

    free(entity);
}

void darken_break()
{
    loop = false;
}
