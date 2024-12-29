#pragma once

#include "config.h"

#define _DE_ASSERT(CONDITION, RETURN, MESSAGE) \
    ({                                        \
        if (CONDITION)                        \
        {                                     \
            DARKEN_LOG(MESSAGE);              \
            return RETURN;                    \
        }                                     \
        RETURN;                               \
    })
