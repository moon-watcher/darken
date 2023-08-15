#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

deEntity_t *deEntity_new(const deState_t *const s)
{
    deEntity_t *e = malloc(sizeof(deEntity_t));

    e->xtor = e->state = (deState_t *)s;
    // e->updateFn = s->update;
    e->manager = 0;

    deEntity_stateEnter(e);

    return e;
}

void deEntity_delete(deEntity_t *const e)
{
    deManager_deleteEntity(e->manager, e);
}

void deEntity_stateSet(deEntity_t *const e, const deState_t *const s)
{
    deEntity_stateLeave(e);
    deEntity_stateForce(e, s);
}

void deEntity_stateForce(deEntity_t *const e, const deState_t *const s)
{
    e->state = (deState_t *)s;

    // if (e->xtor->update == 0)
    //     e->updateFn = s->update;

    deEntity_stateEnter(e);
}

#define EXEC(E, F)      \
    deState_f func = F; \
    if (func != 0)      \
        func(E);

// #define EXEC(E, F)   F && ({F(E); 0;});

void deEntity_stateEnter(deEntity_t *const e)
{
    EXEC(e, e->state->enter);
}

void deEntity_stateUpdate(deEntity_t *const e)
{
    EXEC(e, e->state->update);
}

void deEntity_stateLeave(deEntity_t *const e)
{
    EXEC(e, e->state->leave);
}

void deEntity_stateDestruct(deEntity_t *const e)
{
    deEntity_stateLeave(e);

    if (e->xtor == e->state)
        return;

    EXEC(e, e->xtor->leave);
}