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

    if (iterator != 0 && nbItems != 0)
        funcs[nbItems](list, max, iterator, nbItems);
}

void upiterator_uplist(uplist *const this, void (*iterator)(), unsigned nbItems)
{
    upiterator(this->items, &this->count, iterator, nbItems);
}

void upiterator_culist(culist *const this, void (*iterator)(), unsigned nbItems)
{
    upiterator(this->items, &this->count, iterator, nbItems);
}