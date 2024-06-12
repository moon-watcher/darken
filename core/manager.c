#include "manager.h"

static void _nullf() {}
static const de_state _emptystate = {_nullf, _nullf, _nullf};

static void _destroy(de_entity *const this)
{
    if (this->state->leave != 0)
        this->state->leave(this, this->data);

    if (this->xtor->leave != 0)
        this->xtor->leave(this, this->data);
}

static void _update(de_entity *const this)
{
    this->update(this, this->data);
}

//

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_createEntity(de_manager *const this, de_state *xtor)
{
    de_entity *entity = uplist_alloc(this);

    if (entity != 0)
    {
        entity->update = xtor->update;
        entity->state = (de_state *)&_emptystate;
        entity->xtor = xtor ?: (de_state *)&_emptystate;

        if (entity->xtor->enter != 0)
            entity->xtor->enter(entity, entity->data);
    }

    return entity;
}

int de_manager_deleteEntity(de_manager *const this, de_entity *entity)
{
    if (entity == 0)
        return -2;

    int index = uplist_find(this, entity);

    if (index < 0)
        return -1;

    _destroy(this->items[index]);

    return uplist_remove(this, entity);
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(this, _update, 1);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(this, _destroy, 1);
    uplist_reset(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(this);
}