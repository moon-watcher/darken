#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

deEntity_t *deEntity_new(const deState_t *const s)
{
    deEntity_t *e = malloc(sizeof(deEntity_t));

    e->xtor = e->state = (deState_t *)s;
    e->updateFn = s->update;
    e->manager = 0;

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
    deManager_deleteEntity(e->manager, e);
}
