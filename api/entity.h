#pragma once

#include "../darken.h"

typedef struct deEntityAPI  deEntityAPI;

typedef struct deEntityAPI {
    deEntity_t  *( *const new    ) ( const deState_t *, deManager_t *const );
    deEntityAPI *( *const update ) ( deEntity_t *const );
    deEntityAPI *( *const change ) ( deEntity_t *const, const deState_t *const );
    deEntityAPI *( *const jump   ) ( deEntity_t *const, const deState_t *const );
    deEntityAPI *( *const delete ) ( deEntity_t *const );
} deEntityAPI;

const deEntityAPI  *const deEntity;