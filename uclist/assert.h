#pragma once

#define _UCLIST_ASSERT(EVAL, VALUE) \
    if (EVAL == 0)                  \
    {                               \
        return VALUE;               \
    }
