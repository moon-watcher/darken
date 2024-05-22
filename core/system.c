#include "system.h"

void de_system_init(de_system *const this, void (*update)(), unsigned params)
{
    this->update = update;
    this->params = params ?: 1;

    uplist_init(&this->list);
}

void *de_system_add(de_system *const this, void *const data)
{
    return uplist_add(&this->list, data);
}

unsigned de_system_delete(de_system *const this, void *const data)
{
    return uplist_remove(&this->list, data);
}

void de_system_update(de_system *const this)
{
    uplist_iterator(&this->list, this->update, this->params);
}

void de_system_end(de_system *const this)
{
    uplist_end(&this->list);
}

////////////////////////////////////////////////////////////////////////

// #include "../services/va_arg.h"

// void de_system_addEx(de_system *const this, ...)
// {
//     va_list ap;
//     va_start(ap, this);

//     for (int i = 0; i < this->params; i++)
//         uplist_add(&this->list, va_arg(ap, void *const));
// }

// void de_system_deleteEx(de_system *const this, void *const data)
// {
//     int index = uplist_find(&this->list, data);

//     if (index < 0)
//         return;

//     this->list.count -= this->params;
//     memcpy(this->list.items[index], this->list.items[this->list.count], sizeof(void *) * this->params);
// }