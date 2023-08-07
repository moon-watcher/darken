#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

#include "../config/free.h"
#include "../config/malloc.h"

deEntity_t *deEntity_new(const deState_t *const s, deManager_t *m)
{
    unsigned bytes = sizeof(deEntity_t);

    if (m == 0)
    {
        deEntity_t *e = malloc(bytes);

        memset(e, 0, bytes);

        e->xtor = e->state = (deState_t *)s;
        e->updateFn = s->update;

        deState_enter(e);

        return e;
    }

    bytes += e->manager->maxBytes;

    if (e->manager->free_pos >= e->manager->maxEntities)
        return 0;

    if (e->manager->free_pos >= e->manager->allocated_entities)
    {
        e->manager->entityList[e->manager->free_pos] = malloc(bytes);
        ++e->manager->allocated_entities;
    }

    deEntity_t *e = e->manager->entityList[e->manager->free_pos];

    memset(e, 0, bytes);

    e->index = e->manager->free_pos;
    e->manager->free_pos++;
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
    if (e->manager != 0)
    {
        if (e->index >= e->manager->free_pos)
            return;

        e->manager->free_pos--;

        deEntity_t *const lastEntity = e->manager->entityList[e->manager->free_pos];
        unsigned const lastIndex = lastEntity->index;

        lastEntity->index = e->index;
        e->manager->entityList[e->index] = lastEntity;

        e->index = lastIndex;
        e->manager->entityList[lastIndex] = e;
    }

    deState_leave(e);

    if (e->xtor != e->state)
        deState_exec(e, e->xtor->leave);

    if (e->manager == 0)
        free(e);
}
