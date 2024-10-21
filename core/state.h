#pragma once

typedef struct de_state
{
    void *(*enter)();
    void *(*update)();
    void *(*leave)();
} de_state;