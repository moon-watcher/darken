#include "entity.h"

#include "../services/free.h"
#include "../services/malloc.h"

const de_state de_state_empty = {de_state_func, de_state_func, de_state_func};

__attribute__((always_inline)) inline void de_state_func()
{
    // :)
}

__attribute__((always_inline)) inline void de_state_enter(de_state *const this, de_entity *const entity)
{
    if (this->dm_size != 0)
        entity->statedata = malloc(this->dm_size);

    if (this->enter != 0)
        this->enter(entity, entity->data, entity->statedata);
}

__attribute__((always_inline)) inline void de_state_update(de_state *const this, de_entity *const entity)
{
    if (this->update != 0)
        this->update(entity, entity->data, entity->statedata);
}

__attribute__((always_inline)) inline void de_state_leave(de_state *const this, de_entity *const entity)
{
    if (this->leave != 0)
        this->leave(entity, entity->data, entity->statedata);

    if (this->dm_size != 0)
        free(entity->statedata);

    entity->statedata = 0;
}