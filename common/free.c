#include "../config/free.h"

void de__free(void *ptr)
{
    if (ptr == 0)
        return;

    free(ptr);
    ptr = 0;
}