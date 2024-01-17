#include "entity.h"
#include "../private/state.h"
#include "../private/xtor.h"
#include "../private/entity.h"

de_entity *de_entity_ini(de_entity *const this, const de_state *const xtor)
{
    this->state = (de_state *)&de_state_empty;
    this->xtor = (de_state *)xtor;
    this->update = this->xtor->update ?: de_state_null;
    de_xtor_enter(this);

    return this;
}

de_entity *de_entity_set(de_entity *const this, de_state *const state)
{
    de_state_leave(this);
    this->state = (de_state *)state ?: &de_state_empty;
    de_entity_updatePolicy(this, DARKEN_ENTITY_UPDATEPOLICY_DEFAULT);
    de_state_enter(this);

    return this;
}

void de_entity_updatePolicy(de_entity *const this, unsigned type)
{
    void f0(de_entity *const t) { t->update = t->state->update ?: de_state_null;                     }
    void f1(de_entity *const t) { t->update = t->state->update ?: t->xtor ->update ?: de_state_null; }
    void f2(de_entity *const t) { t->update = t->xtor ->update ?: t->state->update ?: de_state_null; }
    void f3(de_entity *const t) { t->update = t->xtor ->update ?: de_state_null;                     }

    void (*const funcs[])() = {f0, f1, f2, f3};

    funcs[type](this);
}