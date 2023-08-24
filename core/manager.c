#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

#include "../libs/culist.h"

void de_manager_init(de_manager *const m, unsigned int maxEntities, unsigned int objectSize)
{
    culist_init(&m->list, maxEntities, objectSize + sizeof(de_entity));
}

void de_manager_end(de_manager *const m)
{
    culist_end(&m->list, de_state_destruct);
}

void de_manager_reset(de_manager *const m)
{
    culist_reset(&m->list, de_state_destruct);
}

void de_manager_update(de_manager *const m)
{
    culist_iterator(&m->list, de_state_update);
}

de_entity *de_manager_entity_create(de_manager *const m, const de_state *const s)
{
    de_entity *e;
    
    if (m == 0)
        e = malloc(sizeof(de_entity));
    else
    {
        culist *const list = &m->list;
        e = culist_add(list);
        memset(e->data, 0, list->objectSize - sizeof(de_entity));
    }

    e->destructor = s->leave;
    e->manager = m;

    de_state_force(e, s);

    return e;
}

void de_manager_entity_delete(de_manager *const m, de_entity *const e)
{
    if (m != 0)
        culist_remove(&m->list, e, de_state_destruct);
    else
    {
        de_state_destruct(e);
        free(e);
    }
}