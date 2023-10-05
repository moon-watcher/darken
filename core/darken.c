#include "darken.h"

void darken_init(darken *const this, de_state *const state)
{
    this->looper = de_entity_new(state);
    this->entity = 0;
    this->loop = 0;
}

void darken_loop(darken *const this, de_state *const state)
{
    if (this->loop != 0)
        return;

    if (this->entity == 0)
        this->entity = de_entity_new(state);
    else
        de_state_set(this->entity, state);

    this->loop = 1;

    while (this->loop != 0)
        this->looper->state->update(this->entity);
}

void darken_break(darken *const this)
{
    this->loop = 0;
}

void darken_end(darken *const this)
{
    this->loop = 0;

    de_entity_delete(this->looper);
    de_entity_delete(this->entity);
}