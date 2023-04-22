#pragma once

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
