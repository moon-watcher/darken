#pragma once

typedef struct de_entity de_entity;

typedef void (*de_state_f)(de_entity *);

typedef struct de_state
{
    de_state_f enter;
    de_state_f update;
    de_state_f leave;
} de_state;

const de_state de_state_null;

void de_state_enter(de_entity *const);
void de_state_update(de_entity *const);
void de_state_leave(de_entity *const);
void de_state_empty(de_entity *const);
void de_xtor_enter(de_entity *const);
void de_xtor_leave(de_entity *const);

#define de_state_exec(ENTITY, FUNC) \
    if (ENTITY->state->FUNC != 0)   \
        ENTITY->state->FUNC(ENTITY);
