#include "entity.h"

// static void _nullf() {}
// static const de_state _emptystate = {_nullf, _nullf, _nullf};

de_entity *de_entity_set(de_entity *const this, de_state *state)
{
    if (this->state->leave != 0)
        this->state->leave(this, this->data);

    this->state = state;
    // de_entity_setPolicy(this, DARKEN_UPDATEPOLICY);

    if (this->state->enter != 0)
        this->state->enter(this, this->data);

    return this;
}

// void de_entity_setPolicy(de_entity *const this, unsigned type)
// {
//     void *f0(de_entity *const e) { return e->state->update; }
//     void *f1(de_entity *const e) { return e->state->update ?: e->xtor->update; }
//     void *f2(de_entity *const e) { return e->xtor->update ?: e->state->update; }
//     void *f3(de_entity *const e) { return e->xtor->update; }
//     void *(*const funcs[])() = {f0, f1, f2, f3};

//     this->update = funcs[type](this) ?: _nullf;
// }

// void de_entity_updateState(de_entity *const this)
// {
//     this->state->update(this, this->data);
// }