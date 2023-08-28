#include "darken.h"

#include "config/free.h"
#include "config/malloc.h"

void darken(const de_state *const s)
{
    darken_loop(darken_init(s));
}

de_entity *darken_init(const de_state *const s)
{
    if (s == 0)
        return 0;

    de_entity *e = malloc(sizeof(de_entity));

    e->xtor = (de_state *)s;
    e->update = e->xtor->update;

    if (e->xtor->enter != 0)
        e->xtor->enter(e);

    return e;
}

void darken_end(de_entity *const e)
{
    darken_break(e);

    free(e);
}

void darken_state(de_entity *const e, const de_state *const s)
{
    if (s == 0)
        return;

    if (e->xtor != 0 && e->xtor->leave != 0)
        e->xtor->leave(e);

    e->xtor = (de_state *)s;

    if (e->xtor->update != 0)
        e->update = e->xtor->update;
    else
        e->update = ({ void f() {}; f; });

    if (e->xtor->enter != 0)
        e->xtor->enter(e);
}

void darken_update(de_entity *const e)
{
    if (e->xtor != 0 && e->xtor->update != 0)
        e->xtor->update(e);
}

void darken_loop(de_entity *const e)
{
    if (e->xtor == 0)
        return;

    de_state_f const update = e->xtor->update;

    if (update != 0)
        while (e->xtor)
            update(e);
}

void darken_break(de_entity *const e)
{
    if (e->xtor != 0 && e->xtor->enter != 0)
        e->xtor->enter(e);

    e->xtor = 0;
}

// #include "include.h"

// void darken_loop(de_entity *const e)
// {
//     if (e->xtor == 0 || e->xtor->update == 0)
//         return;

//     de_state_f const update = e->xtor->update;

//     while (e->xtor)
//     {
//         drawText("TEXT2", 13, 0);
//         waitMs(500);
//         darken_state(e, &entity_scene_state_test_2);
//         update(e);

//         drawText("TEXT1", 3, 0);
//         waitMs(500);
//         darken_state(e, &entity_scene_state_test_1);
//         update(e);

//         // darken_break(e);
//     }
// }