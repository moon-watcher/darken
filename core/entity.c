#include "../config/darken.h"
// #include "../config/free.h"

#include "state.h"
#include "entity.h"
#include "manager.h"

de_entity* de_entity_set(de_entity *const this, de_state *const state)
{
    if (this->state != 0)
        de_state_leave(this);

    this->state = (de_state *)state ?: &de_state_empty;
    de_entity_updatePolicy(this, DARKEN_ENTITY_UPDATEPOLICY_DEFAULT);
    de_state_enter(this);

    return this;
}

unsigned de_entity_delete(de_entity *const this)
{
    return de_manager_entity_delete(this->manager, this);
}

void de_entity_updatePolicy(de_entity *const this, unsigned type)
{
    void f0(de_entity *const t) { t->update = t->state->update ?: de_state_null; }
    void f1(de_entity *const t) { t->update = t->state->update ?: t->xtor ->update ?: de_state_null; }
    void f2(de_entity *const t) { t->update = t->xtor ->update ?: t->state->update ?: de_state_null; }
    void f3(de_entity *const t) { t->update = t->xtor ->update ?: de_state_null; }
    
    void (*const funcs[])() = {f0, f1, f2, f3};

    funcs[type](this);
}