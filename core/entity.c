#include "entity.h"
#include "../private/state.h"
#include "../private/xtor.h"
#include "../config.h"

de_entity *de_entity_init(de_entity *const this, de_state *xtor)
{
    this->state = &de_state_empty;
    this->xtor = xtor ?: &de_state_empty;
    this->update = this->xtor->update ?: de_state_func;
    dep_xtor_enter(this);

    return this;
}

de_entity *de_entity_set(de_entity *const this, de_state *state)
{
    dep_state_leave(this);
    this->state = state ?: &de_state_empty;
    de_entity_updatePolicy(this, DARKEN_UPDATEPOLICY);
    dep_state_enter(this);

    return this;
}

void de_entity_updatePolicy(de_entity *const this, unsigned type)
{
    de_state_f f0(de_entity *const e) { return e->state->update; }
    de_state_f f1(de_entity *const e) { return e->state->update ?: e->xtor->update; }
    de_state_f f2(de_entity *const e) { return e->xtor->update ?: e->state->update; }
    de_state_f f3(de_entity *const e) { return e->xtor->update; }
    de_state_f (*const funcs[])() = {f0, f1, f2, f3};

    this->update = funcs[type](this) ?: de_state_func;
}

__attribute__((always_inline)) inline void de_entity_update(de_entity *const this)
{
    this->update(this, this->data, this->statedata);
}

void de_entity_destroy(de_entity *const this)
{
    dep_state_leave(this);
    dep_xtor_leave(this);
}
