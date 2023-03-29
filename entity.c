#include <genesis.h>
#include "darken.h"

deEntity_t *deEntity_new(const deDefinition_t *ed)
{
    deEntity_t *e = NULL;
    deManager_t *const m = ed->manager;
    unsigned bytes = sizeof(deEntity_t);
    unsigned zero = 0;
    unsigned *free_pos = &zero;

    if (m == NULL)
        e = malloc(bytes);
    else
    {
        deDefinition_t *const md = (deDefinition_t *const)m->definition;
        free_pos = (unsigned *)&m->free_pos;
        bytes += md->maxBytes;

        if (*free_pos >= max(md->maxEntities, 1))
            return e; // hacer un realloc para que acepte más entidades
        
        if (*free_pos >= m->allocated_entities)
        {
            m->entityList[*free_pos] = malloc(bytes);
            ++m->allocated_entities;
        }

        e = m->entityList[*free_pos];
    }

    memset(e, 0, bytes);
    e->index = *free_pos;
    (*free_pos)++;

    e->state = ed->state;
    e->update = e->state->update;
    e->definition = (deDefinition_t *)ed;

    deState_t *const es = e->state;

    if (es->enter != NULL)
        es->enter(e);

    return e;
}

void deEntity_delete(deEntity_t *e)
{
    deDefinition_t *const ed = e->definition;
    deManager_t *const m = ed->manager;

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
    deState_t *const eds = ed->state;

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