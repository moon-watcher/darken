#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

de_entity *de_entity_new(const de_state *const s)
{
    de_entity *e = malloc(sizeof(de_entity));

    e->destructor = s->leave;
    e->manager = 0;

    de_state_force(e, s);

    return e;
}

void de_entity_delete(de_entity *const e)
{
    de_manager_entity_delete(e->manager, e);
}