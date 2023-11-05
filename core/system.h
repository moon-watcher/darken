#pragma once

#include "../libs/uplist.h"

typedef struct de_system de_system;
typedef void (*de_system_f)();

typedef struct de_system
{
    de_system_f updateFn;
    unsigned int params;
    uplist upl;
    void (*errorHandler)(de_system *const);
} de_system;

void de_system_init(de_system *const, de_system_f const, unsigned int, unsigned int);
void de_system_add(de_system *const, ...);
int de_system_delete(de_system *const, void *const);
void de_system_update(de_system *const);
void de_system_end(de_system *const);