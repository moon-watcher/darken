#pragma once

#include "../darken.h"

typedef struct deSystemAPI deSystemAPI;

typedef struct deSystemAPI {
    deSystemAPI *( *const init   ) ( deSystem_t *const, deSystem_f const, unsigned, unsigned );
    deSystemAPI *( *const update ) ( deSystem_t *const );
    deSystemAPI *( *const end    ) ( deSystem_t *const );
    deSystemAPI *( *const delete ) ( deSystem_t *const, void *const);
    deSystemAPI *( *const add    ) ( deSystem_t *const, void *const[]);
    // deSystemAPI *( *const add    ) ( deSystem_t *const, ...);
} deSystemAPI;

const deSystemAPI *const deSystem;