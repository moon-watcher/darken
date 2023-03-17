#include <genesis.h>
#include "darken.h"

deEntity_t *deEntity_new(const deDefinition_t *ed)
{
    deManager_t *const m = ed->manager;
    deDefinition_t *const md = (deDefinition_t *const)m->definition;
    unsigned *free_pos = (unsigned *)&m->free_pos;
    const unsigned total_bytes = sizeof(deEntity_t) + md->maxBytesEntities;

    if (*free_pos >= max(md->maxEntities, 1))
    {
        // hacer un realloc para
        // que acepte más entidades
        return NULL;
    }

    if (*free_pos >= m->allocated_entities)
    {
        m->entityList[*free_pos] = malloc(total_bytes);
        ++m->allocated_entities;
    }

    deEntity_t *e = m->entityList[*free_pos];

    memset(e, 0, total_bytes);
    e->index = *free_pos;
    (*free_pos)++;

    if (ed->constructor != NULL)
        ed->constructor(e);

    e->state = ed->initialState;
    e->update = e->state->update;
    e->definition = (deDefinition_t *)ed;
    e->state->enter(e);

    return e;
}

void deEntity_delete(deEntity_t *const e)
{
    deDefinition_t *const ed = e->definition;
    deManager_t *const m = ed->manager;
    unsigned *free_pos = (unsigned *)&m->free_pos;

    if (e->index >= *free_pos)
        return;

    (*free_pos)--;
    deEntity_t *const lastdeEntity_t = m->entityList[*free_pos];

    unsigned lastIndex = lastdeEntity_t->index;

    lastdeEntity_t->index = e->index;
    m->entityList[e->index] = lastdeEntity_t;

    e->state->leave(e);

    e->index = lastIndex;
    m->entityList[lastIndex] = e;

    if (ed->destructor != NULL)
        ed->destructor(e);
}

void deEntity_set_state(deEntity_t *const e, const deState_t *const s)
{
    e->state->leave(e);
    e->state = s;
    e->update = e->state->update;
    e->state->enter(e);
}