#include "state.h"
#include "entity.h"



void deState_update(deEntity_t *const e)
{
    DARKEN_STATE_UPDATE(e);
}

void deState_destroy(deEntity_t *const e)
{
    DARKEN_STATE_LEAVE(e);

    if (e->xtor != e->state)
        DARKEN_STATE_EXEC(e, e->xtor->leave);
}
