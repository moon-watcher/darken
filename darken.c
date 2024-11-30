#include "config.h"
#include "core/entity.h"

void darken(de_state (*state)(), unsigned size)
{
    if (state == 0)
    {
        return;
    }

    de_entity *entity = malloc(sizeof(de_entity) + size);
    entity->handler = state;

    while (de_entity_update(entity))
        ;

    free(entity);
}