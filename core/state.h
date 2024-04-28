#pragma once

typedef struct de_entity de_entity;

typedef void (*de_state_f)();

typedef struct de_state
{
    de_state_f enter;
    de_state_f update;
    de_state_f leave;

    unsigned dm_size; // dynamic memory size
} de_state;

void de_state_enter(de_state *const, de_entity *const);
void de_state_update(de_state *const, de_entity *const);
void de_state_leave(de_state *const, de_entity *const);