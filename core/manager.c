#include "manager.h"

#include "../libs/uplist.h"

#include "../private/xtor.h"
#include "../private/state.h"

__attribute__((always_inline)) inline static void _entity_update(de_entity *const this)
{
    this->update(this, this->data);
}

__attribute__((always_inline)) inline static void _entity_destruct(de_entity *const this)
{
    dep_state_leave(this);
    dep_xtor_leave(this);
}

//

void de_manager_init(de_manager *const this, unsigned entityBytes)
{
    uplist_initAlloc(&this->list, entityBytes + sizeof(de_entity));
}

de_entity *de_manager_new(de_manager *const this)
{
    de_entity *entity = uplist_alloc(&this->list);

    entity->update = &de_state_func;
    entity->state = &de_state_empty;
    entity->xtor = &de_state_empty;
    
    return entity;
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(&this->list, _entity_update, 1);
}

unsigned de_manager_delete(de_manager *const this, de_entity *const entity)
{
    int index = uplist_find(&this->list, entity);

    if (index < 0)
        return 0;

    _entity_destruct(this->list.items[index]);

    return uplist_remove(&this->list, entity);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(&this->list, _entity_destruct, 1);
    uplist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(&this->list);
}