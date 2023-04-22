#pragma once

#include "../darken/darken.h"


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
#define entity_change(S)  deEntity_change(e, S);
#define entity_set(S)     deEntity_set(e, S);


    de_define_scene
#define DEFINE_STATE(STATE, ENTER, UPDATE, LEAVE)              \
    static void _##STATE##_enter (deEntity_t *const e) ENTER;  \
    static void _##STATE##_update(deEntity_t *const e) UPDATE; \
    static void _##STATE##_leave (deEntity_t *const e) LEAVE;  \
                                                               \
    const deState_t STATE = {                                  \
        &_##STATE##_enter,                                     \
        &_##STATE##_update,                                    \
        &_##STATE##_leave,                                     \
    };

#define DATA(T) \
    T *const data = (T *const) &e->data

#define REF(T, N, V) \
    T *const V = (T *const) &data->N
    
#define COMP(T, V) \
    T *const V = (T *const) &data->V

#define DEFINE_FUNC_COMPS(NAME, COMPS, IMPL) \
    static void NAME (deEntity_t *const e) { COMPS; IMPL }

#define DEFINE_STATE_COMPS(STATE, COMPS, ENTER, UPDATE, LEAVE) \
    DEFINE_FUNC_COMPS(_##STATE##_enter,  COMPS, ENTER )        \
    DEFINE_FUNC_COMPS(_##STATE##_update, COMPS, UPDATE)        \
    DEFINE_FUNC_COMPS(_##STATE##_leave,  COMPS, LEAVE )        \
                                                               \
    const deState_t STATE = {                                  \
        &_##STATE##_enter,                                     \
        &_##STATE##_update,                                    \
        &_##STATE##_leave,                                     \
    };

#define DEFINE_STATE_COMPS_SYS(STATE, COMPS, SYS, ENTER, UPDATE, LEAVE)        \
    static void _##STATE##_enter  (deEntity_t *const e) { COMPS; ENTER;  SYS } \
    static void _##STATE##_update (deEntity_t *const e) { COMPS; UPDATE; SYS } \
    static void _##STATE##_leave  (deEntity_t *const e) { COMPS; LEAVE       } \
                                                                               \
    const deState_t STATE = {                                                  \
        &_##STATE##_enter,                                                     \
        &_##STATE##_update,                                                    \
        &_##STATE##_leave,                                                     \
    };





void sys_movement_f ( deSystem_t *const );
void sys_sprite_f   ( deSystem_t *const );

#define SystemSprite_add(A, B, C) \
	deSystem_add ( &sys_sprite, (void *const[]){ A, B, C } );
	
#define SystemMovement_add(A, B) \
	deSystem_add ( &sys_movement, (void *const[]){ A, B } );
    
    