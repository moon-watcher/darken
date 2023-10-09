#pragma once

typedef struct de_entity de_entity;

typedef void (*de_state_f)(de_entity *const);

typedef struct de_state
{
    de_state_f enter;
    de_state_f update;
    de_state_f leave;
} de_state;

void de_state_set(de_entity *const, const de_state *const);
void de_state_enter(de_entity *const);
void de_state_update(de_entity *const);
void de_state_leave(de_entity *const);

void *de_state_data(de_entity *const, unsigned int);
void de_state_free(de_entity *const);