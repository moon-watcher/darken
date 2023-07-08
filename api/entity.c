#include "../darken.h"

static deEntityAPI *update(deEntity_t *const a)
{
    deEntity_update(a);
    return (deEntityAPI *)deEntity;
}

static deEntityAPI *change(deEntity_t *const a, const deState_t *const b)
{
    deEntity_setState(a, b);
    return (deEntityAPI *)deEntity;
}

static deEntityAPI *set(deEntity_t *const a, const deState_t *const b)
{
    deEntity_forceState(a, b);
    return (deEntityAPI *)deEntity;
}

static deEntityAPI *delete(deEntity_t *const a)
{
    deEntity_delete(a);
    return (deEntityAPI *)deEntity;
}

const deEntityAPI *const deEntity = &(deEntityAPI){deEntity_new, update, change, set, delete};