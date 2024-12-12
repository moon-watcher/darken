#include "config.h"
#include "core/entity.h"

void darken(de_state_f state, unsigned size)
{
    de_entity *entity = malloc(sizeof(de_entity) + size);

    de_entity_state(entity, state);
    de_entity_destructor(entity, 0);

    while (entity->state != 0)
    {
        entity->state = entity->state(entity->data, entity);
    }

    free(entity);
}
