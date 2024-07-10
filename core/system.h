#pragma once

#include "../libs/uplist.h"

typedef struct
{
    void (*update)();
    unsigned params;
    uplist list;
} de_system;

void de_system_init(de_system *const, void (*)(), unsigned);
void de_system_add(de_system *const, ...);
void de_system_delete(de_system *const, ...);
void de_system_update(de_system *const);
void de_system_end(de_system *const);