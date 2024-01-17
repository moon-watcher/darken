#include "darken.h"
#include "../config/free.h"
#include "../config/malloc.h"
#include "../private/xtor.h"
#include "../private/state.h"

void darken_init(darken *const this, unsigned size)
{
    this->scene = malloc(sizeof(de_entity) + size);
    this->loop = 0;
}
void darken_looper(darken *const this, const de_state *const xtor)
{
    this->scene->xtor = (de_state *)xtor;
}

void darken_state(darken *const this, const de_state *const state)
{
    this->scene->state = (de_state *)state;
}

void darken_break(darken *const this)
{
    this->loop = 0;
}

void darken_loop(darken *const this)
{
    this->loop = 1;
    de_entity *const scene = this->scene;

    dep_xtor_enter(scene);
    dep_state_enter(scene);

    if (scene->xtor->update != 0)
        while (this->loop != 0)
            scene->xtor->update(scene);

    dep_state_leave(scene);
    dep_xtor_leave(scene);
}

void darken_end(darken *const this)
{
    free(this->scene);
    this->loop = 0;
}