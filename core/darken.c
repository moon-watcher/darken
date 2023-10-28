#include "./darken.h"

#include "../config/free.h"
#include "../config/malloc.h"

void darken_init(darken *const this, unsigned int size)
{
    this->scene = malloc(sizeof(de_entity) + size);
    this->loop = 0;
}
void darken_looper(darken *const this, const de_state *const xtor)
{
    this->scene->xtor = xtor;
}

void darken_state(darken *const this, const de_state *const state)
{
    this->scene->state = state;
}

void darken_break(darken *const this)
{
    this->loop = 0;
}

void darken_loop(darken *const this)
{
    this->loop = 1;
    de_entity *const scene = this->scene;

    de_xtor_enter(scene);
    de_entity_enter(scene);

    while (this->loop != 0)
        de_xtor_update(scene);

    de_entity_destruct(scene);
}

void darken_end(darken *const this)
{
    free(this->scene);
    this->loop = 0;
}