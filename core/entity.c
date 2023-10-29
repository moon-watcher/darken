#include "../config/darken.h"
#include "../config/free.h"

#include "state.h"
#include "entity.h"
#include "manager.h"

de_entity *de_entity_set(de_entity *const this, const de_state *const state)
{
    if (state == 0)
        return this;

    if (this->state != 0)
        de_entity_leave(this);

    this->state = (de_state *)state;
    de_entity_set_updateType(this, DARKEN_ENTITY_DEFAULT_UPDATEPOLICY);
    
    return de_entity_enter(this);
}

__attribute__((always_inline)) inline de_entity *de_entity_enter(de_entity *const this)
{
    if (this->state->enter != 0)
        this->state->enter(this);

    return this;
}

__attribute__((always_inline)) inline de_entity *de_entity_update(de_entity *const this)
{
    this->update(this);

    return this;
}

__attribute__((always_inline)) inline de_entity *de_entity_leave(de_entity *const this)
{
    if (this->state->leave != 0)
        this->state->leave(this);

    this->state = 0;
    
#if DARKEN_ENTITY_TEMPDATA
    if(this->tempdata != 0)
    {
        free(this->tempdata);
        this->tempdata = 0;
    }
#endif

    return this;
}

unsigned int de_entity_delete(de_entity *const this)
{
    return de_manager_entity_delete(this->manager, this);
}

void de_entity_destruct(de_entity *const this)
{
    if (this->state != 0)
        de_entity_leave(this);

    if (this->xtor->leave != 0 && this->xtor->leave != this->state->leave)
        this->xtor->leave(this);

    this->xtor = 0;
}

void de_entity_set_updateType(de_entity *const this, unsigned char type)
{
    void nf(de_entity *const t) { };
    void f0(de_entity *const t) { t->update = t->state->update ?: nf; };
    void f1(de_entity *const t) { t->update = t->state->update ?: t->xtor ->update ?: nf; };
    void f2(de_entity *const t) { t->update = t->xtor ->update ?: t->state->update ?: nf; };
    void f3(de_entity *const t) { t->update = t->xtor ->update ?: nf; };
    void (*const funcs[])() = { f0, f1, f2, f3, };

    funcs[type](this);
}