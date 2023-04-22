#pragma once

#include "../darken.h"

typedef struct deManagerAPI deManagerAPI;

typedef struct deManagerAPI {
    deManagerAPI *( *const init   ) ( deManager_t *const, unsigned, unsigned );
    deManagerAPI *( *const update ) ( deManager_t *const );
    deManagerAPI *( *const reset  ) ( deManager_t *const );
    deManagerAPI *( *const end    ) ( deManager_t *const );
} deManagerAPI;

const deManagerAPI *const deManager;