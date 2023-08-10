#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

void deManager_init(deManager_t *const m, unsigned int maxEntities, unsigned int maxBytes)
{
    m->maxBytes = maxBytes ? maxBytes : 1;
    m->maxEntities = maxEntities ? maxEntities : 1;
    m->entityList = malloc(sizeof(deEntity_t *) * m->maxEntities);
    m->freePos = 0;
    m->allocatedEntities = 0;
}

void deManager_end(deManager_t *const m)
{
    unsigned int i = 0;

    for (; i < m->freePos; i++)
    {
        deEntity_t *const e = m->entityList[i];
        deState_leave(e);
        deState_exec(e, e->xtor->leave);
        free(e);
    }

    for (; i < m->allocatedEntities; i++)
        free(m->entityList[i]);

    free(m->entityList);
}

void deManager_reset(deManager_t *const m)
{
    while (m->freePos)
        deEntity_delete(m->entityList[0]);
}

void deManager_update(deManager_t *const m)
{
    if (m->pause == 0)
        for (unsigned int i = 0; i < m->freePos; i++)
            deState_update(m->entityList[i]);

    if (m->pause > 0)
        --m->pause;
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

deEntity_t *deManager_createEntity(deManager_t *const m, const deState_t *const s)
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

deEntity_t *deManager_getEntityByIndex(deManager_t *const m, unsigned int i)
{
    return m->entityList[i];
}

void deManager_deleteEntity(deManager_t *const m, deEntity_t *const e)
{
    if (m != 0)
    {
        if (e->index >= m->freePos)
            return;

        m->freePos--;

        deEntity_t *const lastEntity = m->entityList[m->freePos];
        unsigned int const lastIndex = lastEntity->index;

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
