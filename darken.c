#include "darken.h"

#include "config/free.h"
#include "config/malloc.h"

#define EXEC(F)                          \
    if (e->xtor != 0 && e->xtor->F != 0) \
        e->xtor->F(e);

#define ISVALID                                \
    if (e->xtor == 0 || e->xtor->update == 0) \
        return;

de_entity *darken_init(const de_state *const s)
{
    de_entity *e = malloc(sizeof(de_entity));

    e->xtor = (de_state *)s;
    e->update = e->xtor->update;

    EXEC(enter);

    return e;
}

void darken_end(de_entity *const e)
{
    darken_break(e);
    free(e);
}

void darken_state(de_entity *const e, const de_state *const s)
{
    EXEC(leave);

    e->xtor = (de_state *)s;
    e->update = e->xtor->update;

    EXEC(enter);
}

void darken_update(de_entity *const e)
{
    ISVALID;

    e->xtor->update(e);
}

void darken_loop(de_entity *const e)
{
    ISVALID;

    de_state_f const update = e->xtor->update;

    while (e->xtor)
        update(e);
}

void darken_break(de_entity *const e)
{
    EXEC(leave);
    e->xtor = 0;
}

// #include "include.h"

// void darken_loop(de_entity *const e)
// {
//     ISVALID

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