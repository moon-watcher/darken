#include "../config/malloc.h"

void *de__reserve(void *ptr, unsigned int size)
{
    if (ptr == 0)
        ptr = malloc(size);

    return ptr;
}