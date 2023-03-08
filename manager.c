#include <genesis.h>
#include "darken.h"

void deManager_init(deManager_t *const m, const deDefinition_t *def)
{
    m->entityList = malloc(sizeof(deEntity_t *) * def->maxEntities);
    m->definition = def;
    m->free_pos = 0;
    m->allocated_entities = 0;
}

void deManager_end(deManager_t *const m)
{
    unsigned allocated_entities = m->allocated_entities;
    unsigned free_pos = m->free_pos;
    unsigned i = 0;

    for (; i < free_pos; i++)
    {
        deEntity_t *const e = m->entityList[i];

        e->state->leave(e);

        deDefinition_t *const ed = e->definition;
        
        if (ed->destructor != NULL)
            ed->destructor(e);

        free(e);
    }

    for (; i < allocated_entities; i++)
    {
        deEntity_t *const e = m->entityList[i];
        free(e);
    }

    free(m->entityList);
}

void deManager_update(deManager_t *const m)
{
    unsigned *const free_pos = &m->free_pos;

    for (unsigned i = 0; i < *free_pos; i++)
    {
        deEntity_t *const e = m->entityList[i];
        e->state->update(e);
    }
}