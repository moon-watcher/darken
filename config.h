#pragma once

#include "debug.h"
#define DK_MANAGER(M)                                                \
    ({                                                               \
        de_entity *e = de_manager_new((M));                          \
        if (!e)                                                      \
        {                                                            \
            TRACE                                                    \
            ERROR("Manager `%s`. Error creando entidad.", (M)->name) \
        }                                                            \
        e;                                                           \
    })
