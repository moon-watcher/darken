#include "entity.h"
#include "../config.h"

static void nullf();

de_entity *de_entity_init(de_entity *const this, de_state *const xtor)
{
    this->update = xtor->update ?: nullf;
    this->state = &(de_state){nullf, nullf, nullf};
    this->xtor = xtor;
    de_state_enter(xtor, this);

    return this;
}

de_entity *de_entity_set(de_entity *const this, de_state *const state)
{
    de_state_leave(this->state, this);
    this->state = state;
    de_entity_updatePolicy(this, DARKEN_UPDATEPOLICY);
    de_state_enter(this->state, this);

    return this;
}

void de_entity_updatePolicy(de_entity *const this, unsigned type)
{
    void *f0(de_entity *const e) { return e->state->update; }
    void *f1(de_entity *const e) { return e->state->update ?: e->xtor->update; }
    void *f2(de_entity *const e) { return e->xtor->update ?: e->state->update; }
    void *f3(de_entity *const e) { return e->xtor->update; }
    void *(*const funcs[])() = {f0, f1, f2, f3};

    this->update = funcs[type](this) ?: nullf;
}

//

__attribute__((always_inline)) static inline void nullf()
{
    //
}