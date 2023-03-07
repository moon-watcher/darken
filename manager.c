#include <genesis.h>
#include "darken.h"

void manager_init(Manager *const m, const Managerdef *def)
{
    m->entityList = malloc(sizeof(Entity *) * def->maxEntities);
    m->definition = def;
    m->free_pos = 0;
    m->allocated_entities = 0;
}

void manager_destroy(Manager *const m)
{
    unsigned allocated_entities = m->allocated_entities;
    unsigned free_pos = m->free_pos;
    unsigned i = 0;

    for (; i < free_pos; i++)
    {
        Entity *const e = m->entityList[i];

        e->state->leave(e);

        Entitydef *const ed = e->definition;
        
        if (ed->destructor != NULL)
            ed->destructor(e);

        free(e);
    }

    for (; i < allocated_entities; i++)
    {
        Entity *const e = m->entityList[i];
        free(e);
    }

    free(m->entityList);
}

void manager_update(Manager *const m)
{
    unsigned *const free_pos = &m->free_pos;

    for (unsigned i = 0; i < *free_pos; i++)
    {
        Entity *const e = m->entityList[i];
        e->state->update(e);
    }
}