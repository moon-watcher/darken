#pragma once

#define dehScene_define(NAME, TYPE, ENTER, UPDATE, LEAVE) \
    deh_define_state_function ( NAME##_e, TYPE, ENTER  ); \
    deh_define_state_function ( NAME##_u, TYPE, UPDATE ); \
    deh_define_state_function ( NAME##_l, TYPE, LEAVE  ); \
    const Scene NAME = { & NAME##_e, & NAME##_u, & NAME##_l };

#define dehScene_define_enter(NAME, TYPE, ENTER)          \
    deh_define_state_function ( NAME##_e, TYPE, ENTER );  \
    const Scene NAME = { .enter = & NAME##_e };
    
#define dehScene_define_update(NAME, TYPE, UPDATE)        \
    deh_define_state_function ( NAME##_u, TYPE, UPDATE ); \
    const Scene NAME = { .update = & NAME##_u };
    
#define dehScene_change(S)   deEntity_changeState(this, S)
#define dehScene_set(S)      deEntity_forceState(this, S)
#define dehScene_update()    deState_update(this)
