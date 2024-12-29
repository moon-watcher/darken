#pragma once

#include "config.h"

#define _DARKEN_ASSERT(CONDITION, RETURN, MESSAGE) \
    ({                                        \
        if (CONDITION)                        \
        {                                     \
            DARKEN_LOG(MESSAGE);              \
            return RETURN;                    \
        }                                     \
        RETURN;                               \
    })
