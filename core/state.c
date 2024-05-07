#include "entity.h"
#include "../services/free.h"
#include "../services/malloc.h"

__attribute__((always_inline)) inline void de_state_enter(de_state *const this, de_entity *const entity)
{
    if (this->enter != 0)
        this->enter(entity, entity->data);
}

__attribute__((always_inline)) inline void de_state_update(de_state *const this, de_entity *const entity)
{
    if (this->update != 0)
        this->update(entity, entity->data);
}

__attribute__((always_inline)) inline void de_state_leave(de_state *const this, de_entity *const entity)
{
    if (this->leave != 0)
        this->leave(entity, entity->data);
}