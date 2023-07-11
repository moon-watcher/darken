#pragma once

#define dehScene_define(NAME, TYPE, ENTER, UPDATE, LEAVE) \
    dehState_define(NAME, TYPE, ENTER, UPDATE, LEAVE)

#define dehScene_define_enter(NAME, TYPE, ENTER) \
    dehState_define_enter(NAME, TYPE, ENTER)
    
#define dehScene_define_update(NAME, TYPE, UPDATE)        \
    deh_define_state_function ( NAME##_u, TYPE, UPDATE ); \
    const Scene NAME = { .update = & NAME##_u };
    
#define dehScene_changeState(S) deEntity_changeState(this, S)
#define dehScene_forceState(S)  deEntity_forceState(this, S)
#define dehScene_update()       deState_update(this)

