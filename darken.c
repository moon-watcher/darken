#include "config.h"
#include "core/entity.h"

static bool loop;

void darken_loop(void *(*state)(), unsigned size)
{
    if (state == 0)
    {
        loop = false;
        return;
    }

    de_entity *entity = malloc(sizeof(de_entity) + size);
    entity->handler = state;

    loop = true;
    while (loop == true)
    {
        entity->handler = entity->handler(entity->data, entity);
    }

    free(entity);
}

void darken_break()
{
    loop = false;
}
