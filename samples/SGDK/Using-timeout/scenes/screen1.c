#include "include.h"

static de_entity *player1;
// static de_entity *player2;

static void enter(de_entity *const this)
{
    player1 = de_manager_entity_create(&playersManager, &entity_player_state_xtor);
    // player2 = de_manager_entity_create(&playersManager, &entity_player_state_xtor);

    // struct player *const player1data = (struct player *const)&player1->data;
    // player1data->x = 1;
    // player1data->y = 1;
}

static void update(de_entity *const this)
{
    // update for this scene
}

static void leave(de_entity *const this)
{
    de_entity_delete(player1);
    // de_entity_delete(player2);
}

const de_state entity_scene_state_screen1 = {
    &enter,
    &update,
    &leave,
};