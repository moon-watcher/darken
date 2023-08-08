#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

static deEntity_t *_newLess(const deState_t *const s)
{
    unsigned bytes = sizeof(deEntity_t);

    deEntity_t *e = malloc(bytes);

    memset(e, 0, bytes);

    e->xtor = e->state = (deState_t *)s;
    e->updateFn = s->update;

    deState_enter(e);

    return e;
}

deEntity_t *deEntity_new(const deState_t *const s, deManager_t *m)
{
    if (m == 0)
        return _newLess(s);

    unsigned bytes = sizeof(deEntity_t) + m->maxBytes;
    unsigned *freePos = (unsigned *)&m->freePos;

    if (*freePos >= max(m->maxEntities, 1))
        return 0;

    if (*freePos >= m->allocatedEntities)
    {
        m->entityList[*freePos] = malloc(bytes);
        ++m->allocatedEntities;
    }

    deEntity_t *e = m->entityList[*freePos];

    memset(e, 0, bytes);

    e->index = *freePos;
    (*freePos)++;
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
        unsigned *freePos = (unsigned *)&m->freePos;

        if (e->index >= *freePos)
            return;

        (*freePos)--;

        deEntity_t *const lastEntity = m->entityList[*freePos];
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
