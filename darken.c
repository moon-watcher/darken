#include "darken.h"

static unsigned loop;

void darken_loop(de_state *const state, unsigned size)
{
    loop = 1;
    de_manager_loop(&loop, state, size);
}

void darken_break()
{
    loop = 0;
}