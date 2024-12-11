#include "manager.h"
#include "../config.h"

static void _destroy(de_entity *const entity)
{
    entity->destructor && entity->destructor(entity->data, entity);
}

//

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state_f state)
{
    de_entity *entity = uclist_alloc(this);

    if (entity == 0)
    {
        DARKEN_LOG("de_manager_new: not allocated");
        return 0;
    }

    de_entity_state(entity, state);
    
    return entity;
}

void de_manager_update(de_manager *const this)
{
    for (unsigned index = 0; index < this->count;)
    {
        de_entity_update(this->items[index++]) ?: uclist_removeByIndex(this, --index, _destroy);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(this, _destroy, 1);
    uclist_reset(this);
}

void de_manager_end(de_manager *const this)
{
    uclist_iterator(this, _destroy, 1);
    uclist_end(this);
}
