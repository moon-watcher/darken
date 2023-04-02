#include "darken.h"

inline void deState_exec(deEntity_t *const e, deState_f const func)
{
    if (func != NULL)
        func(e);
}

inline void deState_enter(deEntity_t *const e)
{
    deState_exec(e, e->state->enter);
}

inline void deState_update(deEntity_t *const e)
{
    deState_exec(e, e->state->update);
}

inline void deState_leave(deEntity_t *const e)
{
    deState_exec(e, e->state->leave);
}
