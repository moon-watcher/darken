#pragma once

#define dehState_define(NAME, TYPE, ENTER, UPDATE, LEAVE) \
    deh_define_state_function ( NAME##_e, TYPE, ENTER  ); \
    deh_define_state_function ( NAME##_u, TYPE, UPDATE ); \
    deh_define_state_function ( NAME##_l, TYPE, LEAVE  ); \
    const State NAME = { & NAME##_e, & NAME##_u, & NAME##_l };
    
#define dehState_define_enter(NAME, TYPE, ENTER)          \
    deh_define_state_function ( NAME##_e, TYPE, ENTER );  \
    const State NAME = { .enter = & NAME##_e };

#define dehState_define_update(NAME, TYPE, UPDATE)        \
    deh_define_state_function ( NAME##_u, TYPE, UPDATE ); \
    const State NAME = { .update = & NAME##_u };
    