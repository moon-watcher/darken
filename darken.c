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
    e->update = s->update ?: ({ void f() {}; f; });

    de_state_f const enter = s->enter;

    if (enter != 0)
        enter(e);

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

    de_state_f const leave = e->xtor->leave;

    if (leave != 0)
        leave(e);

    e->xtor = (de_state *)s;
    e->update = s->update ?: ({ void f() {}; f; });

    de_state_f const enter = s->enter;

    if (enter != 0)
        enter(e);
}

void darken_update(de_entity *const e)
{
    de_state_f const update = e->xtor->update;

    if (update != 0)
        update(e);
}

void darken_loop(de_entity *const e)
{
    de_state_f const update = e->xtor->update;

    if (update != 0)
        while (e->xtor != 0)
            update(e);
}

void darken_break(de_entity *const e)
{
    de_state_f const enter = e->xtor->enter;

    if (enter != 0)
        enter(e);

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