#include "include.h"

void VDP_drawText_wrapper(char *string, int *const x, int *const y)
{
    VDP_drawText(string, *x, *y);
}

static void enter(de_entity *const this)
{
    struct player *const data = (struct player *const)&this->data;
    ComponentTimer *const timer = (ComponentTimer *const)&data->timer;
    unsigned int *const x = (unsigned int *const)&data->x;

    *x = 14;
    data->y = 14;

    timer->counter = 0;
    timer->max = 60;
    timer->trigger = &VDP_drawText_wrapper;
    timer->param1 = (void *) "Hello, Timer";
    timer->param2 = (void *) x;
    timer->param3 = (void *) &data->y;

    de_system_add(&sys_timer, timer);
}

static void update(de_entity *const this)
{
    // update for this scene

    struct player *const data = (struct player *const)&this->data;
    ComponentTimer *const timer = (ComponentTimer *const)&data->timer;

    char str[12];
    uintToStr(timer->counter, str, 4);
    VDP_drawText("Counter:", 0, 0);
    VDP_drawText(str, 9, 0);
}

static void leave(de_entity *const this)
{
    struct player *const data = (struct player *const)&this->data;
    ComponentTimer *const timer = (ComponentTimer *const)&data->timer;
    
    de_system_delete(&sys_timer, timer);
}

const de_state entity_player_state_xtor = {
    &enter,
    &update,
    &leave,
};