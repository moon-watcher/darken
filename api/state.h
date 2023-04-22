#pragma once

#include "../darken.h"

typedef struct deStateAPI   deStateAPI;

typedef struct deStateAPI {
    deStateAPI *( *exec   ) ( deEntity_t *const, deState_f const );
    deStateAPI *( *enter  ) ( deEntity_t *const );
    deStateAPI *( *update ) ( deEntity_t *const );
    deStateAPI *( *leave  ) ( deEntity_t *const );
} deStateAPI;

const deStateAPI   *const deState;