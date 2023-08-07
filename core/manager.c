#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

void deManager_init(deManager_t *const m, unsigned int maxEntities, unsigned int maxBytes)
{
    m->maxBytes = maxBytes;
    m->maxEntities = maxEntities;
    m->entityList = malloc(sizeof(deEntity_t *) * max(maxEntities, 1));
    m->free_pos = 0;
    m->allocated_entities = 0;
}

void deManager_end(deManager_t *const m)
{
    unsigned i = 0;
    unsigned entities = m->allocated_entities;
    unsigned free_pos = m->free_pos;

    for (; i < free_pos; i++)
    {
        deEntity_t *const e = m->entityList[i];
        deState_leave(e);
        deState_exec(e, e->xtor->leave);
        free(e);
    }

    for (; i < entities; i++)
        free(m->entityList[i]);

    free(m->entityList);
}

void deManager_reset(deManager_t *const m)
{
    while (m->free_pos)
        deEntity_delete(m->entityList[0]);
}

void deManager_update(deManager_t *const m)
{
    if (m->pause)
    {
        if (m->pause > 0)
            --m->pause;

        return;
    }

    unsigned *const free_pos = &m->free_pos;

    for (unsigned i = 0; i < *free_pos; i++)
    {
        deEntity_t *const e = m->entityList[i];
        deState_update(e);
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

deEntity_t *deManager_newEntity(deManager_t *const m, const deState_t *const s)
{
    return deEntity_new(s, m);
}

deEntity_t *deManager_getEntity(deManager_t *const m, unsigned int index)
{
    return m->entityList[index];
}