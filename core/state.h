#pragma once

typedef void (*de_state_f)();

typedef struct de_state
{
    de_state_f enter;
    de_state_f update;
    de_state_f leave;
} de_state;

#define de_state_enter(ENTITY)  if (ENTITY->state != 0 && ENTITY->state->enter  != 0) ENTITY->state->enter (ENTITY, ENTITY->data);
#define de_state_update(ENTITY) if (ENTITY->state != 0 && ENTITY->state->update != 0) ENTITY->state->update(ENTITY, ENTITY->data);
#define de_state_leave(ENTITY)  if (ENTITY->state != 0 && ENTITY->state->leave  != 0) ENTITY->state->leave (ENTITY, ENTITY->data);
