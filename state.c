#include "state.h"
#include "entity.h"

#define exec(A, B)             \
    deState_f f = A->state->B; \
    if (f) f(A);

void deState_enter  ( deEntity_t *const e ) { exec ( e, enter  ); }
void deState_update ( deEntity_t *const e ) { exec ( e, update ); }
void deState_leave  ( deEntity_t *const e ) { exec ( e, leave  ); }