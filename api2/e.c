#include "../darken.h"
#include "e.h"


// useEntity(e1)->set(&scene_scoring_player_walk);

// useEntity(e1);
// getEntity()->set(&scene_scoring_player_walk);

// useEntity(e1);
// useEntityObj->set(&scene_scoring_player_walk);


static deEntity_t *static_entity;

deEntityAPI2 *useEntity(deEntity_t *const e)
{
    static_entity = e;
    return useEntityObj;
}

deEntityAPI2 *getEntity()
{
    return useEntityObj;
}

static deEntityAPI2 *set(const deState_t *const b)
{
    deEntity_forceState(static_entity, b);
    return useEntityObj;
}


const deEntityAPI2 *const useEntityObj = &(deEntityAPI2){set};