#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

deEntity_t *deEntity_new(const deState_t *const state)
{
    deEntity_t *this = malloc(sizeof(deEntity_t));

    this->xtor = this->state = (deState_t *)state;
    this->updateFn = state->update;
    this->manager = 0;

    deState_enter(this);

    return this;
}

void deEntity_setState(deEntity_t *const this, const deState_t *const state)
{
    deState_leave(this);
    deEntity_forceState(this, state);
}

void deEntity_forceState(deEntity_t *const this, const deState_t *const state)
{
    this->state = (deState_t *)state;

    if (this->xtor->update == 0)
        this->updateFn = state->update;

    deState_enter(this);
}

void deEntity_delete(deEntity_t *const this)
{
    deState_leave(this);

    if (this->xtor != this->state)
        deState_exec(this, this->xtor->leave);

    free(this);
}
