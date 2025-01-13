#include "core/entity.h"

void darken(de_entity *const entity, de_state state)
{
    de_entity_set(entity, state);

    while (state != 0)
    {
        state = state(entity->data, entity);
    }
}
