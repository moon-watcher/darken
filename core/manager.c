#include "manager.h"

static void nullf();
static void update(de_entity *const);
static void destroy(de_entity *const);

void de_manager_init(de_manager *const this, unsigned bytes)
{
    uplist_initAlloc(&this->list, sizeof(de_entity) + bytes);
    this->update = &update;
    this->destroy = &destroy;
}

de_entity *de_manager_new(de_manager *const this)
{
    de_entity *entity = uplist_alloc(&this->list);

    entity->update = nullf;
    entity->state = &(de_state){nullf, nullf, nullf};
    entity->xtor = &(de_state){nullf, nullf, nullf};

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uplist_iterator(&this->list, this->update, 1);
}

unsigned de_manager_delete(de_manager *const this, void *const item)
{
    int index = uplist_find(&this->list, item);

    if (index < 0)
        return 0;

    this->destroy(this->list.items[index]);

    return uplist_remove(&this->list, item);
}

void de_manager_reset(de_manager *const this)
{
    uplist_iterator(&this->list, this->destroy, 1);
    uplist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uplist_end(&this->list);
}

//

__attribute__((always_inline)) static inline void nullf()
{
    //
}

__attribute__((always_inline)) static inline void update(de_entity *const this)
{
    this->update(this, this->data);
}

__attribute__((always_inline)) static inline void destroy(de_entity *const this)
{
    de_state_leave(this->state, this);
    de_state_leave(this->xtor, this);
}