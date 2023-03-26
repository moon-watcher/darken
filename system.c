#include <genesis.h>
#include "system.h"

void deSystem_init(deSystem_t *const s, const deDefinition_t *def)
{
    s->definition = def;
    s->params = def->params;
    s->list = malloc(sizeof(void *) * def->maxItems * def->params);
    s->length = 0;
}

void deSystem_update(deSystem_t *const s)
{
    if (!s || !s->length)
        return;

    deDefinition_t *const def = s->definition;

    if (s->length >= def->maxItems * def->params)
        deSystem_info(s);

    def->updateFn(s);
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
    drawUInt(s->definition->maxItems, 9, 2, 4);
    drawText("LENGTH: ", 0, 3);
    drawUInt(s->length, 9, 3, 4);

    waitMs(20000);
}

void deSystem_add(deSystem_t *const s, void *const array[])
{
    unsigned max = s->params;
    void **const list = s->list;
    
    for (unsigned i = 0; i < max; i++)
        list[s->length++] = array[i];
}