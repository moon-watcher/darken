#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

#include "../libs/dculist.h"

void deManager_init(deManager_t *const m, unsigned int maxEntities, unsigned int objectSize)
{
    dculist_init(&m->list, maxEntities, objectSize + sizeof(deEntity_t));
}

void deManager_end(deManager_t *const m)
{
    dculist_end(&m->list, deState_destruct);
}

void deManager_reset(deManager_t *const m)
{
    dculist_reset(&m->list, deState_destruct);
}

void deManager_update(deManager_t *const m)
{
    dculist_iterator(&m->list, deState_update);
}

deEntity_t *deManager_createEntity(deManager_t *const m, const deState_t *const s)
{
    deEntity_t *e = dculist_add(&m->list);

    memset(e->data, 0, m->list.objectSize - sizeof(deEntity_t));

    e->destructor = s->leave;
    e->manager = m;

    deState_force(e, s);

    return e;
}

void deManager_deleteEntity(deManager_t *const m, deEntity_t *const e)
{
    if (m != 0)
        dculist_remove(&m->list, e, deState_destruct);
    else
    {
        deState_destruct(e);
        free(e);
    }
}