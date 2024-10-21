#include "../core/entity.h"
#include "entity.h"

static void *_nullf() { return 0; }

static void _update(de_entity *const this)
{
    // this->update(this, data);
    void *newstate = this->update(this, this->data);
    ++this->timer;

    if (newstate == 0 || newstate == this->update)
        return;

    // this->state = newstate;
    de_entity_set(this, newstate);
    this->timer = 0;
}

static void _delete(de_entity *const this)
{
    _de_manager_delete(this->manager, this);
}

static void _set(de_entity *const this)
{
    _EXEC(leave, this);

    this->state = this->state ?: &(de_state){_nullf, _nullf, _nullf};
    this->update = this->state->update ?: _nullf;
    this->status = STATUS_UPDATE;

    _EXEC(enter, this);
}

static const void (*const _entity_array[])(de_entity *const) = {_update, _delete, _set};

//

void _de_entity_init(de_entity *const this, de_manager *const manager, void (*desctructor)())
{
    this->update = _nullf;
    this->destructor = desctructor ?: _nullf;
    this->manager = manager;
}

void _de_entity_update(de_entity *const this)
{
    _entity_array[this->status](this);
}

void _de_entity_destroy(de_entity *const this)
{
    _EXEC(leave, this);

    this->destructor(this, this->data);
}