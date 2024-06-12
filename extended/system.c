#include "system.h"
#include "../services/va_arg.h"

void de_system_add_Ex(de_system *const this, ...)
{
    va_list ap;
    va_start(ap, this);

    for (unsigned i = 0; i < this->params; i++)
        uplist_add(&this->list, va_arg(ap, void *const));
}

void de_system_delete_Ex(de_system *const this, void *const data)
{
    int index = uplist_find(&this->list, data);

    if (index < 0)
        return;

    this->list.count -= this->params;
    memcpy(this->list.items[index], this->list.items[this->list.count], sizeof(void *) * this->params);
}