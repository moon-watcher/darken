#include "core/entity.h"

de_entity *darken_init(void *(*alloc)(unsigned short), unsigned size)
{
    return alloc(sizeof(de_entity) + size);
}

void darken_loop(de_entity *const entity, de_state state)
{
    de_entity_set(entity, state);

    while (state != 0)
    {
        state = state(entity->data, entity);
    }
}
