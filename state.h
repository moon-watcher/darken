#pragma once

typedef struct _state
{
    char *name;
    void (*enter)(Entity *const);
    void (*update)(Entity *const);
    void (*leave)(Entity *const);
} State;
