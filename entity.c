#include <genesis.h>
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
            return e; // hacer un realloc para que acepte más entidades
        
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
    e->update = e->xtor->update;

    if (s->enter != NULL)
        s->enter(e);

    return e;
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

    deState_t *const es = e->state;
    deState_t *const eds = e->xtor;

    if (es->leave != NULL)
        es->leave(e);

    if (eds->leave != NULL)
        eds->leave(e);

    if (m == NULL)
        free(e);
}

void deEntity_set_state(deEntity_t *const e, const deState_t *const s)
{
    deState_t *const es = e->state;

    if (es->leave != NULL)
        es->leave(e);

    deEntity_force(e, s);
}

void deEntity_force(deEntity_t *const e, const deState_t *const s)
{
    e->state = s;
    e->update = e->state->update;

    deState_t *const es = e->state;

    if (es->enter != NULL)
        es->enter(e);
}