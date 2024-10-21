#pragma once

typedef struct de_state de_state;

typedef struct de_state
{
    de_state *(*enter)();
    de_state *(*update)();
    de_state *(*leave)();
} de_state;