#pragma once

typedef struct _systemdef
{
    char *name;
    void (*update)();
    unsigned max;
} Systemdef;