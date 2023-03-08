#include <genesis.h>
#include "system.h"

void deSystem_init(deSystem_t *const s, const deDefinition_t *def)
{
    s->definition = def;
    s->list = malloc(sizeof(void *) * def->maxVarsSystem);
    s->length = 0;
}

void deSystem_update(deSystem_t *const s)
{
    if (!s || !s->length)
        return;

    if (s->length >= s->definition->maxVarsSystem)
        deSystem_info(s);

    s->definition->updateSystem(s);
    s->length = 0;
}

void deSystem_end(deSystem_t *const s)
{
    free(s->list);
}

void deSystem_info(deSystem_t *const s)
{
    VDP_resetScreen();

    drawText("=SYSTEM=", 0, 0);
    drawText("NAME:   ", 0, 1);
    drawText(s->definition->name, 9, 1);
    drawText("MAX:    ", 0, 2);
    drawUInt(s->definition->maxVarsSystem, 9, 2, 4);
    drawText("LENGTH: ", 0, 3);
    drawUInt(s->length, 9, 3, 4);

    waitMs(20000);
}

// inline void deSystem_add_var(deSystem_t *const s, void *const v)
// {
//     s->list[s->length++] = v;
// }

// void deSystem_add_var ( deSystem_t *const system, void *const array[] ) {
//     // int t = sizeof (array) / sizeof ( void*const );
// 	while ( *array ) {
//         system->list [ system->length++ ] = *array;
//         ++array;
//     }
// }