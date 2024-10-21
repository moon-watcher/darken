#pragma once

typedef struct de_state de_state;

typedef struct de_state
{
    de_state *(*Handle)();
    //
    void (*Enter)();
    void (*Exit)();
} de_state;
