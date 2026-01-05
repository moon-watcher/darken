#include "darken.h"

void darken_init(de_manager *$, uint16_t bytes)
{
    uclist_init($, sizeof(de_entity) + bytes);
}

de_entity *darken_new(de_manager *$, de_state state)
{
    de_entity *entity = uclist_alloc($);

    return entity->state = state, entity;
}

void darken_update(de_manager *$)
{
    de_entity **items = $->items;
    uint16_t i = $->size;

    while (i--)
    {
        de_entity *entity = items[i];
        de_state state = entity->state;

        if (state > (de_state)DE_PAUSE)
        {
            de_state aux = state(entity->data);

            if (aux != DE_LOOP)
                de_entity_setState(entity, aux);

            continue;
        }

        if (state == (de_state)DE_PAUSE)
        {
            continue;
        }

        // DE_DELETE
        uclist_removeByIndex($, i);
        entity->destructor && entity->destructor(entity->data);
    }
}

void darken_reset(de_manager *$)
{
    uclist_iterator($, ({ void d(de_entity *e) { de_entity_delete(e); }; d; }));
    darken_update($);
}

void darken_end(de_manager *$)
{
    darken_reset($);
    uclist_end($);
}

//

inline void de_entity_setState(de_entity *$, de_state state)
{
    $->state = state;
}

inline de_state de_entity_getState(de_entity *$)
{
    return $->state;
}

inline void de_entity_pause(de_entity *$)
{
    de_entity_setState($, 1);
}

inline void de_entity_delete(de_entity *$)
{
    de_entity_setState($, 0);
}
