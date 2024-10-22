#include "config.h"
#include "core/entity.h"
#include "priv/declarations.h"

static bool loop;

void darken_loop(de_state state, unsigned size)
{
    loop = true;
    
    de_entity *entity = malloc(sizeof(de_entity) + size);
    de_entity_set(entity, state);

    while (loop)
    {
        _entity_update(entity);
    }

    free(entity);
}

void darken_break()
{
    loop = false;
}
