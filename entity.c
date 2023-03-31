#include "darken.h"

deEntity_t *deEntity_new(deState_t *const s, deManager_t *const m)
{
    deEntity_t *e = NULL;
    unsigned bytes = sizeof(deEntity_t);
    unsigned zero = 0;
    unsigned *free_pos = &zero;

    if (m != NULL)
    {
        free_pos = (unsigned *)&m->free_pos;
        bytes += m->maxBytes;

        if (*free_pos >= max(m->maxEntities, 1))
            return NULL;
        
        if (*free_pos >= m->allocated_entities)
        {
            m->entityList[*free_pos] = malloc(bytes);
            ++m->allocated_entities;
        }

        e = m->entityList[*free_pos];
    }
    else
        e = malloc(bytes);

    memset(e, 0, bytes);

    e->index = *free_pos;
    (*free_pos)++;

    e->xtor = s;
    e->state = s;
    
    deState_enter(e);
    
    return e;
}

void deEntity_update(deEntity_t *const e)
{
    deState_f exu = e->xtor->update;
    deState_f func = (exu && e->xtor != e->state) ?
        exu:
        &deState_update;

    func(e);
}

void deEntity_delete(deEntity_t *e)
{
    deManager_t *const m = e->manager;

    if (m)
    {
        unsigned *free_pos = (unsigned *)&m->free_pos;

        if (e->index >= *free_pos)
            return;

        (*free_pos)--;
        deEntity_t *const lastdeEntity = m->entityList[*free_pos];

        unsigned last = lastdeEntity->index;

        lastdeEntity->index = e->index;
        m->entityList[e->index] = lastdeEntity;

        e->index = last;
        m->entityList[last] = e;
    }

    deState_leave(e);
    
    deState_f exlf = e->xtor->leave;    
    if (exlf && e->xtor != e->state)
        exlf(e);

    if (!m)
        free(e);
}

void deEntity_change(deEntity_t *const e, const deState_t *const s)
{
    deState_leave(e);
    deEntity_jump(e, s);
}

void deEntity_jump(deEntity_t *const e, const deState_t *const s)
{
    e->state = (deState_t *)s;
    deState_enter(e);
}