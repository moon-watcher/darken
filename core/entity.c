#include "../config/darken.h"
// #include "../config/free.h"

#include "state.h"
#include "entity.h"
#include "manager.h"

__attribute__((always_inline)) inline void de_entity_set(de_entity *const this, de_state *const state)
{
    const de_state nullstate = {de_state_empty, de_state_empty, de_state_empty};

    if (this->state != 0)
        de_state_leave(this);

    this->state = (de_state *)state ?: &nullstate;
    de_entity_updatePolicy(this, DARKEN_ENTITY_UPDATEPOLICY_DEFAULT);
    de_state_enter(this);
}

__attribute__((always_inline)) inline void de_entity_update(de_entity *const this)
{
    this->update(this);
}

__attribute__((always_inline)) inline unsigned de_entity_delete(de_entity *const this)
{
    return de_manager_entity_delete(this->manager, this);
}

void de_entity_updatePolicy(de_entity *const this, unsigned char type)
{
    void f0(de_entity *const t) { t->update = t->state->update ?: de_state_empty; }
    void f1(de_entity *const t) { t->update = t->state->update ?: t->xtor->update ?: de_state_empty; }
    void f2(de_entity *const t) { t->update = t->xtor->update ?: t->state->update ?: de_state_empty; }
    void f3(de_entity *const t) { t->update = t->xtor->update ?: de_state_empty; }
    
    void (*const funcs[])() = {f0, f1, f2, f3};

    funcs[type](this);
}