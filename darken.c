#include "darken.h"
#include "services/malloc.h"

void darken_infiniteLoop(unsigned size, de_state *const xtor, de_state *const state)
{
    de_entity *const e = malloc(sizeof(de_entity) + size);

    e->xtor = xtor;
    e->state = state;

    if (xtor->enter != 0)
        xtor->enter(e, e->data);

    if (state->enter != 0)
        state->enter(e, e->data);

    while (1)
        xtor->update(e, e->data);
}