#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

static deEntity_t *_newLess(const deState_t *const s)
{
    unsigned bytes = sizeof(deEntity_t);

    deEntity_t *e = malloc(bytes);

    memset(e, 0, bytes);

    e->xtor = e->state = (deState_t *)s;
    e->updateFn = s->update;

    deState_enter(e);

    return e;
}

deEntity_t *deEntity_new(const deState_t *const s, deManager_t *m)
{
    if (m == 0)
        return _newLess(s);

    unsigned bytes = sizeof(deEntity_t) + m->maxBytes;
    unsigned *free_pos = (unsigned *)&m->free_pos;

    if (*free_pos >= max(m->maxEntities, 1))
        return 0;

    if (*free_pos >= m->allocated_entities)
    {
        m->entityList[*free_pos] = malloc(bytes);
        ++m->allocated_entities;
    }

    deEntity_t *e = m->entityList[*free_pos];

    memset(e, 0, bytes);

    e->index = *free_pos;
    (*free_pos)++;
    e->xtor = e->state = (deState_t *)s;
    e->updateFn = s->update;
    e->manager = m;

    deState_enter(e);

    return e;
}

void deEntity_setState(deEntity_t *const e, const deState_t *const s)
{
    deState_leave(e);
    deEntity_forceState(e, s);
}

void deEntity_forceState(deEntity_t *const e, const deState_t *const s)
{
    e->state = (deState_t *)s;

    if (e->xtor->update == 0)
        e->updateFn = s->update;

    deState_enter(e);
}

void deEntity_delete(deEntity_t *const e)
{
    deManager_t *const m = e->manager;

    if (m != 0)
    {
        unsigned *free_pos = (unsigned *)&m->free_pos;

        if (e->index >= *free_pos)
            return;

        (*free_pos)--;

        deEntity_t *const lastEntity = m->entityList[*free_pos];
        unsigned const lastIndex = lastEntity->index;

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
