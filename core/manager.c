#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"
#include "../libs/dculist.h"
static void _delete(deEntity_t *const e);


void deManager_init(deManager_t *const this, unsigned int maxEntities, unsigned int maxBytes)
{
    dculist_init(&this->list, maxEntities, sizeof(deEntity_t) + maxBytes);
    this->pause = 0;
}

void deManager_end(deManager_t *const this)
{
    for (unsigned int i = 0; i < this->list.freePos; i++)
        _delete(this->list.list[i]);

    dculist_end(&this->list);
}

void deManager_reset(deManager_t *const this)
{
    dculist_iterator(&this->list, _delete);
   
    dculist_reset(&this->list);
    this->pause = 0;
}

void deManager_update(deManager_t *const m)
{
    if (m->pause == 0)
        for (unsigned int i = 0; i < m->list.freePos; i++)
        {
            deEntity_t *const e = m->list.list[i];
            deState_update(e);
        }

    if (m->pause > 0)
        --m->pause;
}

void deManager_timeout(deManager_t *const m, unsigned int time)
{
    m->pause = time;
}

void deManager_pause(deManager_t *const m)
{
    m->pause = -1;
}

void deManager_resume(deManager_t *const m)
{
    m->pause = 0;
}

deEntity_t *deManager_createEntity(deManager_t *const m, const deState_t *const s)
{
    deEntity_t *e;

    e = dculist_add(&m->list);
    e->manager = m;
    e->index = m->list.freePos-1;

    e->xtor = e->state = (deState_t *)s;
    e->updateFn = s->update;

    // memset(e->data, 0, sizeof(struc bonus));

    deState_enter(e);

    return e;
}

deEntity_t *deManager_getEntityByIndex(deManager_t *const m, unsigned int i)
{
    return (deEntity_t *) &m->list.list[i];
}
void deManager_deleteEntity(deManager_t *const m, deEntity_t *const e)
{
    deState_leave(e);

    if (e->xtor != e->state)
        deState_exec(e, e->xtor->leave);


        dculist_remove(&m->list, e->index);

        // actualizar el campo index del que hemos movido de LAST a e->index

        // deEntity_t *const lastEntity = m->entityList[m->freePos];
        // unsigned int const lastIndex = lastEntity->index;

        // lastEntity->index = e->index;
        // m->entityList[e->index] = lastEntity;

        // e->index = lastIndex;
        // m->entityList[lastIndex] = e;
 
}


//////////////////


static void _delete(deEntity_t *const e)
{
    deState_leave(e);
    deState_exec(e, e->xtor->leave);
}
