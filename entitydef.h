#pragma once

typedef struct _entitydef
{
    char *name;
    void (*constructor)(Entity *const);
    void (*destructor)(Entity *const);
    Manager *manager;
    const State *initialState;
} Entitydef;
