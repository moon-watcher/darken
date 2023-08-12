#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/free.h"
#include "../config/malloc.h"

#include "../libs/dculist.h"



void deManager_init(deManager_t *const this, unsigned int maxObjects, unsigned int objectSize)
{
    dculist_init(&this->list, maxObjects, sizeof(deEntity_t) + objectSize);
    this->pause = 0;
}

void deManager_end(deManager_t *const this)
{
    dculist_end(&this->list, deState_destroy);
}

void deManager_reset(deManager_t *const this)
{
    dculist_reset(&this->list, deEntity_delete);
}

void deManager_update(deManager_t *const this)
{
    if (this->pause == 0)
        dclist_iterate(&this->list, deState_update);

    if (this->pause > 0)
        --this->pause;
}

void deManager_timeout(deManager_t *const this, unsigned int time)
{
    this->pause = time;
}

void deManager_pause(deManager_t *const this)
{
    this->pause = -1;
}

void deManager_resume(deManager_t *const this)
{
    this->pause = 0;
}

deEntity_t *deManager_createEntity(deManager_t *const this, const deState_t *const state)
{
    deEntity_t *e = dculist_add(&this->list);

    memset(e->data, 0, this->list.objectSize - sizeof(deEntity_t));

    e->index = this->list.freePos - 1;

    e->xtor = e->state = (deState_t *)state;
    e->updateFn = state->update;
    e->manager = this;

    DARKEN_STATE_ENTER(e);

    return e;
}

deEntity_t *deManager_getEntityByIndex(deManager_t *const this, unsigned int index)
{
    return dculist_get(&this->list, index);
}




void deManager_deleteEntity(deManager_t *const this, deEntity_t *const e)
{
    if (this != 0)
    {
        deEntity_t *lastEntity = dculist_delete(&this->list, e->index);

        if (lastEntity == 0)
            return;

        
        unsigned int const lastIndex = lastEntity->index;

        lastEntity->index = e->index;
        this->list.array[e->index] = lastEntity;

        e->index = lastIndex;
        this->list.array[lastIndex] = e;
    }

    DARKEN_STATE_LEAVE(e);

    if (e->xtor != e->state)
        DARKEN_STATE_EXEC(e, e->xtor->leave);

    if (this == 0)
        free(e);
}
