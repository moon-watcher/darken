#pragma once

#include "../darken.h"

typedef struct deEntityAPI2 deEntityAPI2;

typedef struct deEntityAPI2 {
    deEntityAPI2 *( *const set ) ( const deState_t *const );
} deEntityAPI2;

const deEntityAPI2 *const useEntityObj;

deEntityAPI2 *useEntity(deEntity_t *const e);
deEntityAPI2 *getEntity();