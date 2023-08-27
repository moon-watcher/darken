#include "darken.h"
#include "include.h"

static de_manager man;

void darken_init(darken *const de, const de_state *const s)
{
    de->entity = malloc(sizeof(de_entity));

    de->entity->xtor = (de_state *)s;

    if (de->entity->xtor->enter != 0)
        de->entity->xtor->enter(de->entity);

    // de->xtor = s;
    // de->entity = de_entity_new(de->xtor);

    // de_state_f const enter = de->xtor->enter;

    // if (enter != 0)
    //     enter(de->entity);
}

// void darken_update(darken *const de)
// {
//     de_state_f const update = de->xtor->update;
//     de_entity *const entity = de->entity;

//     update(entity);
// }

void darken_state(darken *const de, const de_state *const s)
{
    if (de->entity->xtor->leave != 0)
        de->entity->xtor->leave(de->entity);

    de->entity->xtor = s;

    if (de->entity->xtor->enter != 0)
        de->entity->xtor->enter(de->entity);
}

void darken_loop(darken *const de)
{
    // de->loop = 1;

    // char *const loop = &de->loop;
    // de_state_f const update = de->xtor->update;
    // de_entity *const entity = de->entity;

    // de_state_f *const updatefff = &entity->xtor->update;

    while (1)
         // if (de->entity->xtor->update != 0) de->entity->xtor->update(de->entity);


    {
        drawText("TEXT2", 13, 0); waitMs(500);
        darken_state(de, &entity_scene_state_test_2);
        if (de->entity->xtor->update != 0)
            de->entity->xtor->update(de->entity);

        drawText("TEXT1",  3, 0); waitMs(500);
        darken_state(de, &entity_scene_state_test_1);
        if (de->entity->xtor->update != 0)
            de->entity->xtor->update(de->entity);
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