#pragma once

typedef struct deEntity_t deEntity_t;

typedef void (*deState_f)(deEntity_t *);

typedef struct deState_t
{
    deState_f enter;
    deState_f update;
    deState_f leave;
} deState_t;

#define deState_exec(E, F)  \
    {                       \
        deState_f func = F; \
        if (func != 0)      \
            func(E);        \
    }

#define deState_enter(E)  deState_exec(E, E->state->enter)
#define deState_update(E) deState_exec(E, E->state->update)
#define deState_leave(E)  deState_exec(E, E->state->leave)
