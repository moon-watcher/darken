#include "config.h"
#include "core/entity.h"

void darken(de_state_f state, unsigned size)
{
    de_entity *entity = malloc(sizeof(de_entity) + size);
    de_entity_state(entity, state);

    while (state != 0)
    {
        state = state(entity->data, entity);
    }

    free(entity);
}
