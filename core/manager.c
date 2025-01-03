#include "manager.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state_f state)
{
    return de_entity_set(uclist_alloc(this), state);
}

void de_manager_update(de_manager *const this)
{
    for (unsigned i = 0; i < this->size;)
    {
        de_entity *const entity = this->list[i];

        if (entity->state != 0)
        {
            entity->state = entity->state(entity->data, entity);
            ++i;
            continue;
        }

        --this->size;
        this->list[i] = this->list[this->size];
        this->list[this->size] = entity;
        entity->destructor != 0 && entity->destructor(entity->data, entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(this, de_entity_delete);
    de_manager_update(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uclist_end(this);
}
