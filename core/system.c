#include "../darken.h"

void deSystem_init(deSystem_t *const this, deSystem_f const updateFn, unsigned maxItems, unsigned params)
{
    this->updateFn = updateFn;
    this->maxItems = maxItems;
    this->params = params;

    upl_init(&this->upl, this->maxItems * this->params);
}

void deSystem_add(deSystem_t *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    for (unsigned i = 0; i < this->params; i++)
    {
        int err = upl_add(&this->upl, va_arg(ap, void *const));

        if (err < 0)
        {
            VDP_init();
            VDP_drawText("System: Too much params", 0, 0);
            VDP_drawText(this->name, 0, 1);
            waitMs(10000);

            return;
        }
    }

    va_end(ap);
}

void deSystem_delete(deSystem_t *const this, void *const data)
{
    upl_bulk_removeByData(&this->upl, data, this->params);
}

void deSystem_update(deSystem_t *const this)
{
    upl_bulk_foreach(&this->upl, this->updateFn, this->params);
}

void deSystem_end(deSystem_t *const this)
{
    upl_end(&this->upl);
}
