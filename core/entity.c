#include "entity.h"
#include "../debug.h"

void de_entity_set(de_entity *const this, de_state state)
{
    de_state _nullf()
    {
        return 0;
    }

    this->state = state ?: _nullf;
    this->timer = 0;
}

void de_entity_update(de_entity *const this)
{
    de_state state = this->state(this, this->data);
    ++this->timer;

    if (state != 0)
    {
        de_entity_set(this, state);
    }
}

int de_entity_delete(de_entity *const this)
{
    int ret = uclist_remove(&this->manager->list, this, 0);

#if DARKEN_WARNING
    switch (ret)
    {
    case -1:
        DARKEN_WARNING("Delete entity: entity not found");
        break;
    case -2:
        DARKEN_WARNING("Delete entity: manager, this->count is 0");
        break;
    }
#endif

    return ret;
}
