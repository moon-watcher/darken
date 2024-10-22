#include "../debug.h"
#include "../core/entity.h"
#include "../core/manager.h"

void _entity_init(de_entity *const this, de_manager *const manager, de_state desctructor)
{
    this->state = ({ void *f() { return 0; } f; });
    this->destructor = desctructor;
    this->manager = manager;
}

void _entity_update(de_entity *const this)
{
    de_state state = this->state(this, this->data);
    ++this->timer;

    if (state != 0)
    {
        this->state = state;
        this->timer = 0;
    }
}

void _entity_destroy(de_entity *const this)
{
    if (this->destructor != 0)
        this->destructor(this, this->data);
}

void _manager_delete(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, 0);

    if (ret == -1)
    {
        DARKEN_WARNING("manager, entity not found");
    }
    else if (ret == -2)
    {
        DARKEN_WARNING("manager, this->count is 0");
    }
}
