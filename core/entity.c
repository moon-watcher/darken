#include "../darken.h"

deEntity_t *deEntity_newLess(const deState_t *const s)
{
    unsigned bytes = sizeof(deEntity_t);

    deEntity_t *e = malloc(bytes);

    memset(e, 0, bytes);

    e->xtor = e->state = (deState_t *) s;
    e->updateFn = s->update;

    deState_enter(e);

    return e;
}

deEntity_t *deEntity_new(const deState_t *const s, deManager_t *const m)
{
    unsigned bytes = sizeof(deEntity_t) + m->maxBytes;
    unsigned *free_pos = (unsigned *)&m->free_pos;

    if (*free_pos >= max(m->maxEntities, 1))
        return NULL;
    
    if (*free_pos >= m->allocated_entities)
    {
        m->entityList[*free_pos] = malloc(bytes);
        ++m->allocated_entities;
    }

    deEntity_t *e = m->entityList[*free_pos];

    memset(e, 0, bytes);

    e->index = *free_pos;
    (*free_pos)++;
    e->xtor = e->state = (deState_t *) s;
    e->updateFn = s->update;
    e->manager = m;

    deState_enter(e);

    return e;
}

void deEntity_setState(deEntity_t *const e, const deState_t *const s)
{
    deState_leave(e);
    deEntity_forceState(e, s);
}

void deEntity_forceState(deEntity_t *const e, const deState_t *const s)
{
    e->state = (deState_t *)s;

    if (e->xtor->update == NULL)
        e->updateFn = s->update;

    deState_enter(e);
}

void deEntity_delete(deEntity_t *const e)
{
    deManager_t *const m = e->manager;

    if (m != NULL)
    {
        unsigned *free_pos = (unsigned *)&m->free_pos;

        if (e->index >= *free_pos)
            return;

        (*free_pos)--;

        deEntity_t *const lastEntity = m->entityList[*free_pos];
        unsigned const lastIndex = lastEntity->index;

        lastEntity->index = e->index;
        m->entityList[e->index] = lastEntity;

        e->index = lastIndex;
        m->entityList[lastIndex] = e;
    }

    deState_leave(e);

    if (e->xtor != e->state)
        deState_exec(e, e->xtor->leave);

    if (m == NULL)
        free(e);
}













// #include "darken.h"

// int ypos =0;

// deEntity_t *deEntity_new(const deState_t *s, deManager_t *const m)
// {
//     deEntity_t *e = NULL;
//     unsigned bytes = sizeof(deEntity_t);
//     unsigned zero = 0;
//     unsigned *free_pos = &zero;

//     if (m == NULL)
//     {
//         e = malloc(bytes);
//         drawText("es null", 0,ypos++);
//     }
//     else
//     {
//         drawText("NO es null", 0,ypos++);

//         free_pos = (unsigned *)&m->free_pos;
//         bytes += m->maxBytes;

//         if (*free_pos >= max(m->maxEntities, 1))
//             return NULL;
        
//         if (*free_pos >= m->allocated_entities)
//         {
//             m->entityList[*free_pos] = malloc(bytes);
//             ++m->allocated_entities;
//         }

//         e = m->entityList[*free_pos];
//     }

//     memset(e, 0, bytes);

// drawUInt(m->free_pos, 0,ypos++,3);
// drawUInt(*free_pos,   0,ypos++,3);

//     e->index = *free_pos;
//     (*free_pos)++;
//     e->xtor = e->state = s;
//     e->updateFn = s->update;
//     e->manager = m;

//     deState_enter(e);


// drawUInt(m->free_pos, 0,ypos++,3);
// drawUInt(*free_pos,   0,ypos++,3);
// waitMs(1000);

//     return e;
// }

// void deEntity_update(deEntity_t *const e)
// {
//     deState_exec(e, e->updateFn);
// }

// void deEntity_setState(deEntity_t *const e, const deState_t *const s)
// {
//     deState_leave(e);
//     deEntity_forceState(e, s);
// }

// void deEntity_forceState(deEntity_t *const e, const deState_t *const s)
// {
//     e->state = (deState_t *)s;

//     if (e->xtor->update == NULL)
//         e->updateFn = (deState_f *)s->update;

//     deState_enter(e);
// }

// void deEntity_delete(deEntity_t *const e)
// {
//     deManager_t *const m = e->manager;

//     if (m != NULL)
//     {
//         drawText("NO es null", 0,ypos++);
//         unsigned *free_pos = (unsigned *)&m->free_pos;

// drawUInt(m->free_pos, 0,ypos++,3);
// drawUInt(*free_pos,   0,ypos++,3);

//         if (e->index >= *free_pos)
//             return;

//         (*free_pos)--;

//         deEntity_t *const lastEntity = m->entityList[*free_pos];
//         unsigned const lastIndex = lastEntity->index;

//         lastEntity->index = e->index;
//         m->entityList[e->index] = lastEntity;

//         e->index = lastIndex;
//         m->entityList[lastIndex] = e;
        
//         drawUInt(m->free_pos, 0,ypos++,3);
//         drawUInt(*free_pos,   0,ypos++,3);
//     }
//     else
//     {
//         drawText("es null", 0,ypos++);
//     }



//     deState_leave(e);

//     if (e->xtor != e->state)
//         deState_exec(e, e->xtor->leave);

//     if (m == NULL)
//         free(e);
// }


