#include "darken.h"

const DarkenAPI    *const darkEn    = & ( DarkenAPI    ) { darken, darken_end };
const deSystemAPI  *const deSystem  = & ( deSystemAPI  ) { deSystem_init,  deSystem_update,    deSystem_end    };
const deManagerAPI *const deManager = & ( deManagerAPI ) { deManager_init, deManager_update,   deManager_end   };
const deEntityAPI  *const deEntity  = & ( deEntityAPI  ) { deEntity_new,   deEntity_set_state, deEntity_delete };
