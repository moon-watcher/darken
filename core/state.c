#include "state.h"
#include "entity.h"
// #include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"


void deState_destruct(deEntity_t *const e)
{
    DARKEN_STATE_LEAVE(e);
    
    if (e->xtor != e->state)
        DARKEN_STATE_EXEC(e, e->xtor->leave);
}

void deState_update(deEntity_t *const e)
{
    DARKEN_STATE_UPDATE(e);
}