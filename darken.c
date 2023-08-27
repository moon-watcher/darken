#include "darken.h"
#include "include.h"

// static de_manager man;

de_entity *darken_init(const de_state *const s)
{
    de_entity *e = malloc(sizeof(de_entity));

    e->xtor = (de_state *)s;

    de_state_f const enter = e->xtor->enter;
    
    if (enter != 0)
        enter(e);

    return e;
}


void darken_state(de_entity *const e, const de_state *const s)
{
    de_state_f const leave = e->xtor->leave;

    if (leave != 0)
        leave(e);

    e->xtor = s;

    de_state_f const enter = e->xtor->enter;
    
    if (enter != 0)
        enter(e);
}

void darken_loop(de_entity *const e)
{
    // de->loop = 1;

    // char *const loop = &de->loop;
    // de_state_f const update = de->xtor->update;
    // de_entity *const entity = de->entity;

    // de_state_f *const updatefff = &entity->xtor->update;

    while (1)
         // if (e->xtor->update != 0) e->xtor->update(e);


    {
        drawText("TEXT2", 13, 0); waitMs(500);
        darken_state(e, &entity_scene_state_test_2);
        if (e->xtor->update != 0)
            e->xtor->update(e);

        drawText("TEXT1",  3, 0); waitMs(500);
        darken_state(e, &entity_scene_state_test_1);
        if (e->xtor->update != 0)
            e->xtor->update(e);
    }
}


// void darken_end(darken *const de)
// {
//     de_entity_delete(de->entity);
//     de->entity = 0;
//     de->xtor = 0;
//     de->loop = 0;
// }


// void darken_loop(de_entity *const e)
// {
//     int uno = 1;
//     e->xtor = &uno;
//     de_state_f *const update = &e->update;

//     // while (e->xtor)
//     //     ((*update)(e));
//     //     e->update(e);

//     while (e->xtor)
//     {
//         drawText("TEXT2", 13, 0);
//         waitMs(500);
//         de_state_set(e, &entity_scene_state_test_2);
//         ((*update)(e));

//         drawText("TEXT1", 3, 0);
//         waitMs(500);
//         de_state_set(e, &entity_scene_state_test_1);
//         ((*update)(e));

//         // darken_break(e);
//     }
// }