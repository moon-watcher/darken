#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

#include "../libs/dculist.h"

void deManager_init(deManager_t *const m, unsigned int maxEntities, unsigned int objectSize)
{
    dculist_init(&m->list, maxEntities, objectSize + sizeof(deEntity_t));
    m->pause = 0;
}

void deManager_end(deManager_t *const m)
{
    dculist_end(&m->list, deEntity_stateDestruct);
}

void deManager_reset(deManager_t *const m)
{
    dculist_reset(&m->list, deEntity_stateDestruct);
}

void deManager_update(deManager_t *const m)
{
    if (m->pause == 0)
        dculist_iterator(&m->list, deEntity_stateUpdate);

    if (m->pause > 0)
        --m->pause;
}

deEntity_t *deManager_createEntity(deManager_t *const m, const deState_t *const s)
{
    deEntity_t *e = dculist_add(&m->list);

    memset(e->data, 0, m->list.objectSize - sizeof(deEntity_t));

    e->xtor = e->state = (deState_t *)s;
    // e->updateFn = s->update;
    e->manager = m;

    deEntity_stateEnter(e);

    return e;
}

void deManager_deleteEntity(deManager_t *const m, deEntity_t *const e)
{
    if (m != 0)
        dculist_remove(&m->list, e, deEntity_stateDestruct);
    else
    {
        deEntity_stateDestruct(e);
        free(e);
    }
}

void deManager_timeout(deManager_t *const m, unsigned int time)
{
    m->pause = time;
}

void deManager_pause(deManager_t *const m)
{
    m->pause = -1;
}

void deManager_resume(deManager_t *const m)
{
    m->pause = 0;
}