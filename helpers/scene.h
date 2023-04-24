#pragma once

#define dehScene_define(NAME, ENTER, UPDATE, LEAVE)            \
    static void NAME##_e (deEntity_t *const this) ENTER        \
    static void NAME##_u (deEntity_t *const this) UPDATE       \
    static void NAME##_l (deEntity_t *const this) LEAVE        \
                                                               \
    const Scene NAME = { & NAME##_e, & NAME##_u, & NAME##_l };
    
#define dehScene_define_enter(NAME, ENTER)               \
    static void NAME##_e (deEntity_t *const this) ENTER  \
    const deState_t NAME = { .enter = & NAME##_e };
    
#define dehScene_define_update(NAME, UPDATE)             \
    static void NAME##_u (deEntity_t *const this) UPDATE \
    const deState_t NAME = { .update = & NAME##_u };
    
#define dehScene_change(S)   deEntity_change(this, S)
#define dehScene_set(S)      deEntity_set(this, S)
#define dehScene_update()    deState_update(this)
