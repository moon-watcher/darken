#include "entity.h"
#include "../services/free.h"
#include "../services/malloc.h"

__attribute__((always_inline)) inline void de_state_enter(de_state *const this, de_entity *const entity)
{
    this->enter(entity, entity->data);
}

__attribute__((always_inline)) inline void de_state_update(de_state *const this, de_entity *const entity)
{
    this->update(entity, entity->data);
}

__attribute__((always_inline)) inline void de_state_leave(de_state *const this, de_entity *const entity)
{
    this->leave(entity, entity->data);
}