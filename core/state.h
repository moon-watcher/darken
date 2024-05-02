#pragma once

typedef struct de_entity de_entity;

typedef struct
{
    void (*enter)();
    void (*update)();
    void (*leave)();
} de_state;

void de_state_enter(de_state *const, de_entity *const);
void de_state_update(de_state *const, de_entity *const);
void de_state_leave(de_state *const, de_entity *const);