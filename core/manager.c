#include "manager.h"

static void destroy(de_entity *const this)
{
    de_state_leave(this->state, this);
    de_state_leave(this->xtor, this);
}

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(this, sizeof(de_entity) + bytes);
}

de_entity *de_manager_new(de_manager *const this, de_state *const xtor)
{
    de_entity *entity = uplist_alloc(this);

    entity->update = xtor->update;
    entity->xtor = xtor;

    de_state_enter(xtor, entity);

    return entity;
}

void de_manager_update(de_manager *const this)
{
    void update(de_entity *const this)
    {
        this->update(this, this->data);
    }

    uplist_iterator(this, update, 1);
}

unsigned de_manager_delete(de_manager *const this, void *const item)
{
    int index = uplist_find(this, item);

    if (index < 0)
        return 0;

    destroy(this->items[index]);

    return uplist_remove(this, item);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(this, destroy, 1);
    uplist_reset(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(this);
}