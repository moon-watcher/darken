#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

deEntity_t *deEntity_new(const deState_t *const s, deManager_t *m)
{
    deEntity_t *e;

    if (m == 0)
        e = malloc(sizeof(deEntity_t));
    else
    {
        if (m->freePos >= m->maxEntities)
            return 0;

        if (m->freePos >= m->allocatedEntities)
        {
            m->entityList[m->freePos] = malloc(sizeof(deEntity_t) + m->maxBytes);
            ++m->allocatedEntities;
        }

        e = m->entityList[m->freePos];

        memset(e->data, 0, m->maxBytes);

        e->index = m->freePos;
        m->freePos++;
    }

    e->xtor = e->state = (deState_t *)s;
    e->updateFn = s->update;
    e->manager = m;

    deState_enter(e);

    return e;
}

void deEntity_setState(deEntity_t *const e, const deState_t *const s)
{
    deState_leave(e);
    deEntity_forceState(e, s);
}

void deEntity_forceState(deEntity_t *const e, const deState_t *const s)
{
    e->state = (deState_t *)s;

    if (e->xtor->update == 0)
        e->updateFn = s->update;

    deState_enter(e);
}

void deEntity_delete(deEntity_t *const e)
{
    deManager_t *const m = e->manager;

    if (m != 0)
    {
        if (e->index >= m->freePos)
            return;

        m->freePos--;

        deEntity_t *const lastEntity = m->entityList[m->freePos];
        unsigned const lastIndex = lastEntity->index;

        lastEntity->index = e->index;
        m->entityList[e->index] = lastEntity;

        e->index = lastIndex;
        m->entityList[lastIndex] = e;
    }

    deState_leave(e);

    if (e->xtor != e->state)
        deState_exec(e, e->xtor->leave);

    if (m == 0)
        free(e);
}
