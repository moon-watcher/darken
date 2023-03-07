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

    for (unsigned i = 0; i < allocated_entities; i++)
    {
        Entity *e = m->entityList[i];

        if (i < free_pos)
        {
            e->state->leave(e);

            Entitydef *const ed = e->definition;

            if (ed->destructor != NULL)
                ed->destructor(e);
        }

        free(e);
    }

    free(m->entityList);
}

void manager_update(Manager *const m)
{
    unsigned pos = m->free_pos;

    for (unsigned i = 0; i < pos; i++)
    {
        Entity *const e = m->entityList[i];
        e->state->update(e);
    }
}