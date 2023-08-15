#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

deEntity_t *deEntity_new(const deState_t *const s)
{
    deEntity_t *e = malloc(sizeof(deEntity_t));

    e->destructor = s->leave;
    e->manager = 0;

    deState_force(e, s);

    return e;
}

void deEntity_delete(deEntity_t *const e)
{
    deManager_deleteEntity(e->manager, e);
}