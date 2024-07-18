#include "darken.h"

static unsigned loop;

void darken_loop(unsigned size, de_state *state)
{
    loop = 1;
    de_manager_loop(&loop, state, size);
}

void darken_break()
{
    loop = 0;
}