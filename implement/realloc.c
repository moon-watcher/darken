#include "../config/malloc.h"
#include "../config/free.h"

void *de_implement_realloc(void *ptr, unsigned int size)
{
    if (ptr == 0)
        return 0;

    void *p = malloc(size);

    if (p == 0)
        return 0;
    
    // unsigned int originalSize = getsize(ptr);
    // if (originalSize < size)
    //     size = originalSize;

    memcpy(p, ptr, size);
    free(ptr);
    
    return p;
}
