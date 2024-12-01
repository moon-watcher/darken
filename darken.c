#include "config.h"
#include "core/entity.h"

void darken(de_state (*state)(), unsigned size)
{
    de_entity *entity = malloc(sizeof(de_entity) + size);
    de_entity_set(entity, state);

    while (de_entity_update(entity))
        ;

    free(entity);
}