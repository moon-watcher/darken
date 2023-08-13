#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

void dculist_init(dculist_t *const this, unsigned int maxEntities, unsigned int maxBytes)
{
    this->maxBytes = maxBytes ? maxBytes : 1;
    this->maxEntities = maxEntities ? maxEntities : 1;
    this->entityList = malloc(sizeof(void *) * this->maxEntities);
    this->freePos = 0;
    this->allocatedEntities = 0;
}

void dculist_iterator(dculist_t *const this, void (*callback)(void *const))
{
    if (callback != 0)
        for (unsigned int i = 0; i < this->freePos; i++)
            callback(this->entityList[i]);
}

void dculist_end(dculist_t *const this, void (*callback)(void *const))
{
    dculist_iterator(this, callback);

    for (unsigned int i = 0; i < this->allocatedEntities; i++)
        free(this->entityList[i]);

    free(this->entityList);
}

void dculist_reset(dculist_t *const this, void (*callback)(void *const))
{
    dculist_iterator(this, callback);

    this->freePos = 0;
}

void *dculist_get(dculist_t *const this, unsigned int i)
{
    return this->entityList[i];
}

void *dculist_new(dculist_t *const this)
{
    if (this->freePos >= this->maxEntities)
        return 0;

    if (this->freePos >= this->allocatedEntities)
    {
        this->entityList[this->freePos] = malloc(this->maxBytes);
        ++this->allocatedEntities;
    }

    this->freePos++;

    return this->entityList[this->freePos - 1];
}

int dculist_lastIndex(dculist_t *const this)
{
    return this->freePos - 1;
}

int dculist_find(dculist_t *const this, void *const data)
{
    for (unsigned int i = 0; i < this->freePos; i++)
        if (this->entityList[i] == data)
            return i;
    
    return -1;
}

unsigned int dculist_remove(dculist_t *const this, void *const data, void (*callback)(void *const))
{
    int const index = dculist_find(this, data);

    if (index < 0)
        return 0;

    this->freePos--;

    if (callback)
        callback(this->entityList[index]);

    this->entityList[index] = this->entityList[this->freePos];

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void deManager_init(deManager_t *const m, unsigned int maxEntities, unsigned int maxBytes)
{
    dculist_init(&m->list, maxEntities, sizeof(deEntity_t) + maxBytes);
    m->pause = 0;
}

void deManager_end(deManager_t *const m)
{
    dculist_end(&m->list, deState_destruct);
}

void deManager_reset(deManager_t *const m)
{
    dculist_reset(&m->list, deState_destruct);
}

void deManager_update(deManager_t *const m)
{
    if (m->pause == 0)
        dculist_iterator(&m->list, deState_update);

    if (m->pause > 0)
        --m->pause;
}

deEntity_t *deManager_createEntity(deManager_t *const m, const deState_t *const s)
{
    deEntity_t *e = dculist_new(&m->list);

    memset(e->data, 0, m->list.maxBytes - sizeof(deEntity_t));

    e->xtor = e->state = (deState_t *)s;
    e->updateFn = s->update;
    e->manager = m;

    DARKEN_STATE_ENTER(e);

    return e;
}

void deManager_deleteEntity(deManager_t *const m, deEntity_t *const e)
{
    if (m != 0)
        dculist_remove(&m->list, e, deState_destruct);
    else
    {
        deState_destruct(e);
        free(e);
    }
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