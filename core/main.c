#include "main.h"
#include "../services/free.h"
#include "../services/malloc.h"

void de_main_init(darken *this, unsigned size)
{
    this->scene = malloc(sizeof(de_entity) + size);
    this->loop = 0;
}

void de_main_looper(darken *this, de_state *xtor)
{
    this->scene->xtor = xtor;
}

void de_main_state(darken *this, de_state *state)
{
    this->scene->state = state;
}

void de_main_break(darken *this)
{
    this->loop = 0;
}

void de_main_loop(darken *this)
{
    this->loop = 1;
    de_entity *const scene = this->scene;

    de_state_enter(scene->xtor, scene);
    de_state_enter(scene->state, scene);

    if (scene->xtor->update != 0)
        while (this->loop != 0)
            scene->xtor->update(scene, scene->data, scene->statedata);

    de_state_leave(scene->state, scene);
    de_state_leave(scene->xtor, scene);
}

void de_main_end(darken *this)
{
    free(this->scene);
    this->loop = 0;
}