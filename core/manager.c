#include "manager.h"

static void _destroy(de_entity *const entity)
{
    if (entity != 0 && entity->destructor != 0)
    {
        entity->destructor(entity->data, entity);
    }
}

//

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state_f state)
{
    return de_entity_state(uclist_alloc(this), state);
}

void de_manager_update(de_manager *const this)
{
    for (unsigned index = 0; index < this->count;)
    {
        de_entity *const entity = this->list[index];

        if (entity->state != 0)
        {
            entity->state = entity->state(entity->data, entity);
            ++index;
        }
        else
        {
            --this->count;
            this->list[index] = this->list[this->count];
            this->list[this->count] = entity;
            _destroy(entity);
        }
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(this, _destroy);
    uclist_reset(this);
}

void de_manager_end(de_manager *const this)
{
    uclist_iterator(this, _destroy);
    uclist_end(this);
}
