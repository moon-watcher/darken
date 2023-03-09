#include "darken.h"

static int exitCode = 0;

const DarkenAPI    *const darken    = & ( DarkenAPI    ) { darken_init, darken_end };
const deSystemAPI  *const deSystem  = & ( deSystemAPI  ) { deSystem_init,  deSystem_update,    deSystem_end    };
const deManagerAPI *const deManager = & ( deManagerAPI ) { deManager_init, deManager_update,   deManager_end   };
const deEntityAPI  *const deEntity  = & ( deEntityAPI  ) { deEntity_new,   deEntity_set_state, deEntity_delete };



int darken_init(const deState_t *initialState, void (*constructor)(deEntity_t *const))
{
    exitCode = 0;
    int *x = &exitCode;

    deManager_t manager;
    const deDefinition_t ed = {.name = "Darken main entity", .manager = &manager, .initialState = initialState, .constructor = constructor};
    const deDefinition_t md = {.name = "Darken main manager", .maxEntities = 1};

    deManager_init(&manager, &md);
    deEntity_t *const e = deEntity_new(&ed);

    while (!*x)
        e->state->update(e);

    deManager_end(&manager);

    return exitCode;
}

void darken_end(int code)
{
    exitCode = code;
}