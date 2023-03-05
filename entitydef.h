#pragma once

typedef struct _entitydef
{
    char *name;
    void (*constructor)();
    void (*destructor)();
    Manager *manager;
    const State *initialState;
} Entitydef;
