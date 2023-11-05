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

    if (scene->xtor->enter != 0)
        scene->xtor->enter(scene);
    
    if (scene->state->enter != 0)
        scene->state->enter(scene);

    if (scene->xtor->update != 0)
        while (this->loop != 0)
            scene->xtor->update(scene);

    if (scene->state != 0)
        de_state_leave(scene);

    if (scene->xtor->leave != 0 && scene->xtor->leave != scene->state->leave)
        scene->xtor->leave(scene);

    scene->xtor = 0;
}

void darken_end(darken *const this)
{
    free(this->scene);
    this->loop = 0;
}