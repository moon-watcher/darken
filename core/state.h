#pragma once

typedef struct de_entity de_entity;

typedef void (*de_state_f)(de_entity *);

typedef struct de_state
{
    de_state_f enter;
    de_state_f update;
    de_state_f leave;
} de_state;

const de_state de_state_empty;

void de_state_update(de_entity *const);
void de_state_null(de_entity *const);