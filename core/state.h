#pragma once

typedef struct deEntity_t deEntity_t;

typedef void (*deState_f)(deEntity_t *);

typedef struct deState_t
{
    deState_f enter;
    deState_f update;
    deState_f leave;
} deState_t;

#define DARKEN_STATE_EXEC(E, F) \
    {                           \
        deState_f func = F;     \
        if (func != 0)          \
            func(E);            \
    }

#define DARKEN_STATE_ENTER(E)  DARKEN_STATE_EXEC ( E, E->state->enter  )
#define DARKEN_STATE_UPDATE(E) DARKEN_STATE_EXEC ( E, E->state->update )
#define DARKEN_STATE_LEAVE(E)  DARKEN_STATE_EXEC ( E, E->state->leave  )

void deState_destruct(deEntity_t *const entity);
void deState_update(deEntity_t *const entity);