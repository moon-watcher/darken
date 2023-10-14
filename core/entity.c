#include "include.h"

#include "state.h"
#include "entity.h"
#include "manager.h"

#include "../config/darken.h"
#include "../config/free.h"
#include "../config/malloc.h"

de_entity *de_entity_new(const de_state *const state, unsigned int size)
{

// #if DARKEN_ENTITY_DATA
    size += sizeof(de_entity);
// #else
//     size = sizeof(de_entity);
// #endif


size = 500;
    de_entity *entity = malloc(size);

// #if DARKEN_ENTITY_DATA
//     memset(entity->data, 0, size-sizeof(de_entity));
// #endif

VDP_drawText("creada",0,dev0++); // waitMs(1000);

    entity->manager = 0;
    entity->state = (de_state *)state;
    
    de_state_enter(entity);
    
VDP_drawText("set",0,dev0++); waitMs(100);
    
    return entity;
}

void de_entity_delete(de_entity *this)
{
    if (this->manager != 0)
        return de_manager_entity_delete(this->manager, this);

    de_state_leave(this);
    free(this);
}