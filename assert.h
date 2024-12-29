#pragma once

#include "config.h"

#define _DARKEN_ASSERT(EVAL, RETURN, MESSAGE) \
    _DARKEN_EQUAL(EVAL == 0, RETURN, MESSAGE)

#define _DARKEN_EQUAL(CONDITION, RETURN, MESSAGE) \
    ({                                         \
        if (CONDITION)                         \
        {                                      \
            DARKEN_LOG(MESSAGE);               \
            return RETURN;                     \
        }                                      \
        RETURN;                                \
    })
