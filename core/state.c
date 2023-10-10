#include "state.h"
#include "entity.h"

void de_state_set(de_entity *const entity, const de_state *const newState)
{
    if (entity->state == newState)
        return;

    de_state_leave(entity);
    entity->state = (de_state *)newState;
    de_state_enter(entity);
}

void de_state_enter(de_entity *const entity)
{
    if(entity->state->enter != 0)
        entity->state->enter(entity);
}

void de_state_update(de_entity *const entity)
{
    entity->state->update(entity);
}

void de_state_leave(de_entity *const entity)
{
    if(entity->state->leave != 0)
        entity->state->leave(entity);
}