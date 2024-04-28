#pragma once

typedef struct de_entity de_entity;

typedef struct de_state
{
    void (*enter)();
    void (*update)();
    void (*leave)();

    unsigned dm_size; // dynamic memory size
} de_state;

void de_state_enter(de_state *const, de_entity *const);
void de_state_update(de_state *const, de_entity *const);
void de_state_leave(de_state *const, de_entity *const);