#include "./darken.h"

void darken_init(darken *const this)
{
    this->looper = 0;
    this->entity = 0;
    this->loop = 0;
}

void darken_loop(darken *const this)
{
    unsigned char *const loop = &this->loop;

    if (*loop != 3)
        return;
    
    de_entity *const entity = this->entity;
    de_entity *const looper = this->looper;

    while (*loop == 3)
        looper->state->update(entity);

    de_state_leave(entity);
    de_state_leave(looper);

    darken_init(this);
}

void darken_break(darken *const this)
{
    this->loop = 0;
}

void darken_end(darken *const this)
{
    darken_break(this);
    
    de_entity_delete(this->looper);
    de_entity_delete(this->entity);
}

void darken_looper(darken *const this, de_state *const state)
{
    this->loop |= 1 << 0;
    
    if (this->looper == 0)
        this->looper = de_entity_new(state);
    else
        de_state_set(this->looper, state);
}

void darken_state(darken *const this, de_state *const state)
{
    this->loop |= 1 << 1;

    if (this->entity == 0)
        this->entity = de_entity_new(state);
    else
        de_state_set(this->entity, state);
}