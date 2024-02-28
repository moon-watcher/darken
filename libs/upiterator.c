#include "upiterator.h"

#define IT(F, ...)                                                                 \
    static void F(void **const list, unsigned *const n, void (*it)(), unsigned nb) \
    {                                                                              \
        for (unsigned i = 0; i < *n; i += nb)                                      \
            it(__VA_ARGS__);                                                       \
    }

IT(f0, );
IT(f1, list[i + 0]);
IT(f2, list[i + 0], list[i + 1]);
IT(f3, list[i + 0], list[i + 1], list[i + 2]);
IT(f4, list[i + 0], list[i + 1], list[i + 2], list[i + 3]);
IT(f5, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);
IT(f6, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5]);
IT(f7, list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6]);

void upiterator(void **const list, unsigned *const max, void (*iterator)(), unsigned nbItems)
{
    static void (*const funcs[])() = {f0, f1, f2, f3, f4, f5, f6, f7};

    if (iterator && nbItems)
        funcs[nbItems](list, max, iterator, nbItems);
}


// void culist_iterator(culist *const this, void (*callback)())
// {
//     if (callback == 0)
//         return;

//     void **const list = this->upl.list;
//     unsigned *const next = &this->upl.next;

//     for (unsigned i = 0; i < *next; i++)
//         callback(list[i]);
// }


// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     void **const list = this->list;
//     unsigned *const next = &this->next;

//     for (unsigned i = 0; i < *next; i += nbItems)
//         iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
// }


// void uplist_iterator(uplist *const this, void (*iterator)(), unsigned nbItems)
// {
//     if (iterator == 0 || nbItems == 0)
//         return;

//     unsigned const items = nbItems;

//     void **const list = this->list;
//     unsigned *const next = &this->next;

//     if (items == 1)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0]);

//     else if (items == 2)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1]);

//     else if (items == 3)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2]);

//     else if (items == 4)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3]);

//     else if (items == 5)
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4]);

//     else
//         for (unsigned i = 0; i < *next; i += items)
//             iterator(list[i + 0], list[i + 1], list[i + 2], list[i + 3], list[i + 4], list[i + 5], list[i + 6], list[i + 7], list[i + 8], list[i + 9], list[i + 10]);
// }


// void de_manager_iterate(de_manager *const this, void (*iterator)())
// {
//     // culist_iterator(&this->cul, iterator);
//     upiterator(this->cul.upl.list, &this->cul.upl.next, iterator, 1);
// }


// void culist_iterator(culist *const this, void (*iterator)())
// {
//     upiterator(this->upl.list, &this->upl.next, iterator, 1);
// }

// void culist_iteratorEx(culist *const this, void (*iterator)(), unsigned params)
// {
//     upiterator(this->upl.list, &this->upl.next, iterator, params);
// }


// void uplist_iteratorEx(uplist *const this, void (*iterator)(), unsigned nbItems)
// {
//     iterator(this->list, nbItems);
// }
