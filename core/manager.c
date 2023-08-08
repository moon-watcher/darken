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
    int i = 0;

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
        for (int i = 0; i < m->freePos; i++)
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

deEntity_t *deManager_newEntity(deManager_t *const m, const deState_t *const s)
{
    return deEntity_new(s, m);
}

deEntity_t *deManager_getEntity(deManager_t *const m, unsigned int index)
{
    return m->entityList[index];
}