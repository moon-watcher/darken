#include "entity.h"
#include "manager.h"
#include "state.h"
#include <genesis.h>

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
    
    deState_f exe = e->xtor->enter;
    if (exe) exe(e);

    return e;
}

void deEntity_update(deEntity_t *const e)
{
    deState_f exu = e->xtor->update;
    deState_f f = &deState_update;

    if (exu && e->xtor != e->state)
        f = exu;
    
    f(e);
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

    deState_f esl = e->state->leave;
    deState_f exl = e->xtor->leave;
    
    if (esl) esl(e);
    if (exl && e->xtor != e->state) exl(e);

    if (m == NULL)
        free(e);
}

void deEntity_change(deEntity_t *const e, const deState_t *const s)
{
    deState_f esl = e->state->leave;

    if (esl) esl(e);

    deEntity_force(e, s);
}

void deEntity_force(deEntity_t *const e, const deState_t *const s)
{
    e->state = (deState_t *)s;
    
    deState_f ese = e->state->enter;
    
    if (ese) ese(e);
}