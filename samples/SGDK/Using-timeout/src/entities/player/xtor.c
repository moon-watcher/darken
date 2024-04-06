#include "include.h"

static void enter(de_entity *const this)
{
    struct player *const data = (struct player *const)&this->data;
    ComponentTimer *const timer = (ComponentTimer *const)&data->timer;

    go_system_add(timerSystem, timer);
}

static void update(de_entity *const this)
{
    struct player *const data = (struct player *const)&this->data;
    ComponentTimer *const timer = (ComponentTimer *const)&data->timer;

    char str[12];
    uintToStr(timer->counter, str, 4);
    VDP_drawText("Counter:", 0, 2);
    VDP_drawText(str, 9, 2);
}

static void leave(de_entity *const this)
{
    struct player *const data = (struct player *const)&this->data;
    ComponentTimer *const timer = (ComponentTimer *const)&data->timer;

    go_system_delete(timerSystem, timer);
}

const de_state entity_player_state_xtor = {
    &enter,
    &update,
    &leave,
};