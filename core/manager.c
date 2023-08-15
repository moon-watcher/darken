#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

#include "../libs/dculist.h"

void de_manager_init(de_manager *const m, unsigned int maxEntities, unsigned int objectSize)
{
    de_libs_dculist_init(&m->list, maxEntities, objectSize + sizeof(de_entity));
}

void de_manager_end(de_manager *const m)
{
    de_libs_dculist_end(&m->list, de_state_destruct);
}

void de_manager_reset(de_manager *const m)
{
    de_libs_dculist_reset(&m->list, de_state_destruct);
}

void de_manager_update(de_manager *const m)
{
    de_libs_dculist_iterator(&m->list, de_state_update);
}

de_entity *de_manager_entity_create(de_manager *const m, const de_state *const s)
{
    de_entity *e = de_libs_dculist_add(&m->list);

    memset(e->data, 0, m->list.objectSize - sizeof(de_entity));

    e->destructor = s->leave;
    e->manager = m;

    de_state_force(e, s);

    return e;
}

void de_manager_entity_delete(de_manager *const m, de_entity *const e)
{
    if (m != 0)
        de_libs_dculist_remove(&m->list, e, de_state_destruct);
    else
    {
        de_state_destruct(e);
        free(e);
    }
}