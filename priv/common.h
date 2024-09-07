#pragma once

#if DARKEN_DEBUG
#define _DARKEN_COMMON_INIT(THIS, DATASIZE)                    \
    if (DATASIZE == 0)                                         \
    {                                                          \
        DARKEN_DEBUG_NOTICE("manager/system datasize is 0");   \
    }                                                          \
    else if ((THIS->data = malloc(DATASIZE)) == 0)             \
    {                                                          \
        DARKEN_DEBUG_ERROR("manager/system malloc() is null"); \
    }
#else
#define _DARKEN_COMMON_INIT(THIS, DATASIZE) \
    DATASIZE && (THIS->data = malloc(DATASIZE))
#endif

#define _DARKEN_COMMON_END(THIS, TYPE) \
    uclist_end(&THIS->list);           \
    free(THIS->data);                  \
    memset(THIS, 0, sizeof(TYPE));
