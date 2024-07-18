#pragma once

typedef struct de_state
{
    void (*enter)();
    void (*update)();
    void (*leave)();
} de_state;

const de_state de_state_empty;

void de_state_nullf();