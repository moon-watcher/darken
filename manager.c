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
    unsigned max = m->definition->maxEntities;
    unsigned pos = m->free_pos;

    for (unsigned i = 0; i < max; i++)
    {
        Entity *const e = m->entityList[i];

        if (i < pos)
        {
            e->state->leave(e);

            Entitydef *const ed = e->definition;

            if (ed->destructor)
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