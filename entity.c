#include "entity.h"
#include "manager.h"
#include <genesis.h>

#define exec(A, B) if (A->B != NULL) A->B(A)

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
    exec(e, xtor->enter);

    return e;
}

void deEntity_update(deEntity_t *const e)
{
    if (e->xtor != e->state)
        exec(e, xtor->update);

    exec(e, state->update);
}

void deEntity_delete(deEntity_t *e)
{
    deManager_t *const m = e->manager;

    if (m != NULL)
    {
        unsigned *free_pos = (unsigned *)&m->free_pos;

        if (e->index >= *free_pos)
            return;

        (*free_pos)--;
        deEntity_t *const lastdeEntity = m->entityList[*free_pos];

        unsigned lastIndex = lastdeEntity->index;

        lastdeEntity->index = e->index;
        m->entityList[e->index] = lastdeEntity;

        e->index = lastIndex;
        m->entityList[lastIndex] = e;
    }

    exec(e, state->leave);

    if (e->xtor != e->state)
        exec(e, xtor->leave);

    if (m == NULL)
        free(e);
}

void deEntity_change(deEntity_t *const e, const deState_t *const s)
{
    exec(e, state->leave);
    deEntity_force(e, s);
}

void deEntity_force(deEntity_t *const e, const deState_t *const s)
{
    e->state = (deState_t *)s;
    exec(e, state->enter);
}