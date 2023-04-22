#pragma once

typedef deState_t Scene;

#define DEFINE_SCENE(SCENE, ENTER, UPDATE, LEAVE)              \
    static void _##SCENE##_enter (deEntity_t *const e) ENTER;  \
    static void _##SCENE##_update(deEntity_t *const e) UPDATE; \
    static void _##SCENE##_leave (deEntity_t *const e) LEAVE;  \
                                                               \
    const Scene SCENE = {                                      \
        &_##SCENE##_enter,                                     \
        &_##SCENE##_update,                                    \
        &_##SCENE##_leave,                                     \
    };



#define scene_change(S)   deEntity_change(e, S);
#define scene_set(S)      deEntity_set(e, S);