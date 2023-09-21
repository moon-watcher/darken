#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/malloc.h"

de_entity *de_entity_new(const de_state *const s)
{
    return de_manager_entity_create(0, s);
}

void de_entity_delete(de_entity *const e)
{
    de_manager_entity_delete(e->manager, e);
}

#if DARKEN_ENTITY_TMP
    void *de_entity_tmp(de_entity *const e, unsigned int size)
    {
        return e->tmp = malloc(size);
    }
#endif