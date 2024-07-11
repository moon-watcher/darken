#include "darken.h"

static int loop;

void darken_loop(unsigned size, de_state *state)
{
    loop = 1;
    size += sizeof(de_entity);

    de_manager m;
    de_manager_init(&m, size);
    de_entity_set(de_manager_new(&m, 0), state);

    while (loop == 1)
        de_manager_update(&m);

    de_manager_end(&m);
}

void darken_break()
{
    loop = 0;
}