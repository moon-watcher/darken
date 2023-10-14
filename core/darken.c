#include "./darken.h"

void darken_init(darken *const this)
{
    this->scene = 0;
    this->loop = 0;
}

void darken_scene(darken *const this, de_scene *const newscene, unsigned int size)
{
    this->loop = 1;
    
    // if (this->scene != 0)
    //     de_entity_delete(this->scene);
// #include "include.h"
// dev0 = 11;
// VDP_drawText("1AEEHola",0,3); waitMs(2000);

    this->scene = de_entity_new(newscene, size);
}

void darken_loop(darken *const this)
{
    unsigned char *const loop = &this->loop;

    if (*loop != 1)
        return;

    de_entity *const scene = this->scene;
    
    while (*loop == 1)
        scene->state->update(scene);

    de_state_leave(scene);

    darken_init(this);
}

void darken_break(darken *const this)
{
    this->loop = 0;
}

void darken_end(darken *const this)
{
    darken_break(this);

    de_entity_delete(this->scene);
}