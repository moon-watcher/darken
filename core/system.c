#include "../darken.h"

void deSystem_init(deSystem_t *const s, deSystem_f const updateFn, unsigned maxItems, unsigned params)
{
    s->updateFn = updateFn;
    s->maxItems = maxItems;
    s->params = params;
    s->list = malloc(sizeof(void *) * maxItems * params);
    s->length = 0;
}

void deSystem_update(deSystem_t *const s)
{
    if (s == NULL || s->length == 0)
        return;

    if (s->length >= s->maxItems * s->params)
        return;

    s->updateFn(s);
    s->length = 0;
}

void deSystem_end(deSystem_t *const s)
{
    free(s->list);
}

void deSystem_add(deSystem_t *const s, void *const array[])
{
    unsigned const max = s->params;
    void **const list = s->list;
    
    for (unsigned i = 0; i < max; i++)
        list[s->length++] = array[i];
}

void deSystem_info(deSystem_t *const s)
{
    VDP_resetScreen();

    drawText("=SYSTEM=", 0, 0);
    drawText("MAX:    ", 0, 1);
    drawUInt(s->maxItems, 9, 1, 4);
    drawText("LENGTH: ", 0, 2);
    drawUInt(s->length, 9, 2, 4);

    waitMs(20000);
}