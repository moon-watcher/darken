#pragma once

typedef struct de_entity de_entity;

typedef struct de_state
{
    void (*enter)(de_entity *const);
    void (*update)(de_entity *const);
    void (*leave)(de_entity *const);
} de_state;

void de_state_set(de_entity *const, const de_state *const);
void de_state_enter(de_entity *const);
void de_state_update(de_entity *const);
void de_state_leave(de_entity *const);

typedef de_state de_scene;
typedef de_state de_looper;