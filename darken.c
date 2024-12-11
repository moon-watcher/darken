#include "config.h"
#include "core/entity.h"

void darken(de_state_f state, unsigned size)
{
    de_entity *entity = malloc(sizeof(de_entity) + size);

    de_entity_state(entity, state);
    de_entity_destructor(entity, 0);

    while (de_entity_update(entity))
        ;

    free(entity);
}
