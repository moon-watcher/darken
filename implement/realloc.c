#include "../config/malloc.h"
#include "../config/free.h"

void *realloc(void *ptr, unsigned int size)
{
    void *p = malloc(size);

    if (p != 0 && ptr != 0)
    {
        // unsigned int originalSize = ¿¿¿getsize(ptr)???;
        // if (originalSize < size)
        //     size = originalSize;

        memcpy(p, ptr, size);
        free(ptr);
    }

    return p;
}
