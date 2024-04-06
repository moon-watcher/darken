#include "include.h"

static void constructor(de_entity *const this)
{
    // Initialization stuff
    de_manager_init(&playersManager, sizeof(struct player));
    go_system_init(timerSystem, &sys_timer_f, 1);

    // And jump to some scene state
    de_entity_set(this, &scene_screen1);
}

static void mainLoopUpdater(de_entity *const this)
{
    // de_entity_update(this);
    this->update(this);

    de_manager_update(&playersManager);
    go_system_update(timerSystem);

    SPR_update();
    SYS_doVBlankProcess();
}

static void destructor(de_entity *const this)
{
    de_manager_end(&playersManager);
    go_system_end(timerSystem);
}

const de_state scene_xtor = {
    &constructor,
    &mainLoopUpdater,
    &destructor,
};