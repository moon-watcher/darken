#include "darken.h"

void deManager_init(deManager_t *const m, unsigned maxEntities, unsigned maxBytes)
{
    m->maxBytes = maxBytes;
    m->maxEntities = maxEntities;
    m->entityList = malloc(sizeof(deEntity_t *) * max(maxEntities, 1));
    m->free_pos = 0;
    m->allocated_entities = 0;
}

void deManager_end(deManager_t *const m)
{
    unsigned allocated_entities = m->allocated_entities;
    unsigned free_pos = m->free_pos;
    unsigned i = 0;

    for (; i < free_pos; i++)
    {
        deEntity_t *const e = m->entityList[i];

        deState_leave(e);

        deState_f leave = e->xtor->leave;

        if (leave)
            leave(e);

        free(e);
    }

    for (; i < allocated_entities; i++)
        free(m->entityList[i]);

    free(m->entityList);
}

void deManager_update(deManager_t *const m)
{
    unsigned *const free_pos = &m->free_pos;

    for (unsigned i = 0; i < *free_pos; i++)
        deEntity_update(m->entityList[i]);
}