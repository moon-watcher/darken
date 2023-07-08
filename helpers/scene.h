#pragma once

#define dehScene_define(NAME, TYPE, ENTER, UPDATE, LEAVE) \
    dehState_define(NAME, TYPE, ENTER, UPDATE, LEAVE)

#define dehScene_define_enter(NAME, TYPE, ENTER) \
    dehState_define_enter(NAME, TYPE, ENTER)
    
#define dehScene_define_update(NAME, TYPE, UPDATE) \
    dehState_define_update(NAME, TYPE, UPDATE)

#define dehScene_setState(S)   deEntity_setState(this, S)
#define dehScene_forceState(S) deEntity_forceState(this, S)
#define dehScene_update()      deState_update(this)


typedef deState_t Scene;
typedef deState_t deScene_t;
typedef deState_t *const SceneRef;