#pragma once

#define dehState_define(NAME, TYPE, ENTER, UPDATE, LEAVE)      \
    deh_define_state_function ( NAME##_enter,  TYPE, ENTER  ); \
    deh_define_state_function ( NAME##_update, TYPE, UPDATE ); \
    deh_define_state_function ( NAME##_leave,  TYPE, LEAVE  ); \
    const deState_t NAME = { & NAME##_enter, & NAME##_update, & NAME##_leave };
    
#define dehState_define_enter(NAME, TYPE, ENTER)             \
    deh_define_state_function ( NAME##_enter, TYPE, ENTER ); \
    const deState_t NAME = { .enter = & NAME##_enter };

#define dehState_define_update(NAME, TYPE, UPDATE)             \
    deh_define_state_function ( NAME##_update, TYPE, UPDATE ); \
    const deState_t NAME = { .update = & NAME##_update };
    