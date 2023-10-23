#include "include.h"

static de_entity *p1;

static void VDP_drawText_wrapper(char *string, int *const x, int *const y)
{
    VDP_drawText(string, *x, *y);
}

static void enter(de_entity *const this)
{
    p1 = de_manager_entity_new(&playersManager, &entity_player_state_xtor);

    struct player *const p1data = (struct player *const)&p1->data;
    ComponentTimer *const p1timer = (ComponentTimer *const)&p1data->timer;
    unsigned int *const p1x = (unsigned int *const)&p1data->x;

    *p1x = 14;
    p1data->y = 14;

    p1timer->counter = 0;
    p1timer->max = 60;
    p1timer->trigger = &VDP_drawText_wrapper;
    p1timer->param1 = (void *) "Hello, Timer";
    p1timer->param2 = (void *) p1x;
    p1timer->param3 = (void *) &p1data->y;
}

static void update(de_entity *const this)
{
    // update for this scene
}

static void leave(de_entity *const this)
{
    de_entity_delete(p1);
}

const de_state scene_screen1 = {
    &enter,
    &update,
    &leave,
};