#pragma once

#define dehState_define(NAME, ENTER, UPDATE, LEAVE)                \
    static void NAME##_e (deEntity_t *const this) ENTER            \
    static void NAME##_u (deEntity_t *const this) UPDATE           \
    static void NAME##_l (deEntity_t *const this) LEAVE            \
    const deState_t NAME = { & NAME##_e, & NAME##_u, & NAME##_l };
    
#define dehState_define_enter(NAME, ENTER)               \
    static void NAME##_e (deEntity_t *const this) ENTER  \
    const deState_t NAME = { .enter = & NAME##_e };

#define dehState_define_update(NAME, UPDATE)             \
    static void NAME##_u (deEntity_t *const this) UPDATE \
    const deState_t NAME = { .update = & NAME##_u };