#include <genesis.h>
#include "darken.h"

Entity *entity_new(const Entitydef *ed)
{
    Manager *const m = ed->manager;
    Managerdef *const md = (Managerdef *const)m->definition;
    unsigned *free_pos = (unsigned *)&m->free_pos;
    const unsigned total_bytes = sizeof(Entity) + md->entities_max_bytes;

    if (*free_pos >= md->maxEntities)
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

    Entity *e = m->entityList[*free_pos];

    memset(e, 0, total_bytes);
    e->index = *free_pos;
    (*free_pos)++;

    if (ed->constructor != NULL)
        ed->constructor(e);

    e->state = ed->initialState;
    e->definition = (Entitydef *)ed;
    e->state->enter(e);

    return e;
}

void entity_delete(Entity *const e)
{
    Entitydef *const ed = e->definition;
    Manager *const m = ed->manager;
    unsigned *free_pos = (unsigned *)&m->free_pos;

    if (e->index >= *free_pos)
        return;

    (*free_pos)--;
    Entity *const lastEntity = m->entityList[*free_pos];
    lastEntity->index = e->index;
    m->entityList[e->index] = lastEntity;

    e->state->leave(e);

    if (ed->destructor != NULL)
        ed->destructor(e);
}

void entity_state(Entity *const e, const State *const s)
{
    e->state->leave(e);
    e->state = s;
    e->state->enter(e);
}
