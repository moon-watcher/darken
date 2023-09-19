#pragma once

#include <genesis.h>
#include "darken/darken.h"
#include "components/Timer.h"

const de_state entity_scene_state_xtor;
const de_state entity_scene_state_screen1;
const de_state entity_player_state_xtor;
const de_state entity_player_state_walk;

de_manager playersManager;

de_system sys_timer;

void sys_timer_f(ComponentTimer *const);

struct player {
    ComponentTimer timer;
    int x, y;
};