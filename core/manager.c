#include "../darken.h"

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uclist_init(this, DARKEN_SIZE(bytes));
}

de_entity *de_manager_new(de_manager *const this, de_state state)
{
    return de_entity_set(uclist_alloc(this), state);
}

void de_manager_update(de_manager *const this)
{
    for (unsigned i = 0; i < this->size;)
    {
        de_entity *const entity = this->list[i++];

        if (entity->state == 0)
        {
            uclist_removeByIndex(this, --i, 0);
            de_entity_set(entity, entity->destructor ?: ({ void *f(){return 0;} f; }));
        }

        entity->state = entity->state(entity->data, entity);
    }
}

void de_manager_reset(de_manager *const this)
{
    uclist_iterator(this, (void *)de_entity_delete);
    de_manager_update(this);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uclist_end(this);
}
