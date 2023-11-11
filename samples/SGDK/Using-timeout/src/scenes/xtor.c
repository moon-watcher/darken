#include "include.h"

static void constructor(de_entity *const this)
{
    // Initialization stuff
    de_manager_init(&playersManager, 20, sizeof(struct player));
    de_system_init(&sys_timer, &sys_timer_f, 1);

    // And jump to some scene state
    de_entity_set(this, &scene_screen1);
}

static void mainLoopUpdater(de_entity *const this)
{
    // de_entity_update(this);
    this->update(this);

    de_manager_update(&playersManager);
    de_system_update(&sys_timer);

    SPR_update();
    SYS_doVBlankProcess();
}

static void destructor(de_entity *const this)
{
    de_manager_end(&playersManager);
    de_system_end(&sys_timer);
}

const de_state scene_xtor = {
    &constructor,
    &mainLoopUpdater,
    &destructor,
};