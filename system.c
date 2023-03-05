#include <genesis.h>
#include "system.h"

void system_init(System *const s, const Systemdef *template)
{
    s->def = template;
    s->list = malloc(sizeof(void *) * template->max);
    s->length = 0;
}

void system_update(System *const s)
{
    if (!s || !s->length)
        return;

    if (s->length >= s->def->max)
        system_info(s);

    s->def->update(s);
    s->length = 0;
}

void system_destroy(System *const s)
{
    free(s->list);
}

void system_info(System *const s)
{
    VDP_resetScreen();

    drawText("=SYSTEM=", 0, 0);
    drawText("NAME:   ", 0, 1);
    drawText(s->def->name, 9, 1);
    drawText("MAX:    ", 0, 2);
    drawUInt(s->def->max, 9, 2, 4);
    drawText("LENGTH: ", 0, 3);
    drawUInt(s->length, 9, 3, 4);

    waitMs(20000);
}

// inline void system_add(System *const s, void *const v)
// {
//     s->list[s->length++] = v;
// }

// void system_add ( System *const system, void *const array[] ) {
//     // int t = sizeof (array) / sizeof ( void*const );
// 	while ( *array ) {
//         system->list [ system->length++ ] = *array;
//         ++array;
//     }
// }