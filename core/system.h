#pragma once

#include "../libs/uplist.h"

typedef struct
{
    void (*update)();
    unsigned params;
    void *data;
    uplist list;
} de_system;

void de_system_init(de_system *const, void (*)(), unsigned, unsigned);
void de_system_add(de_system *const, ...);
void de_system_delete(de_system *const, ...);
void de_system_update(de_system *const);
void de_system_reset(de_system *const);
unsigned de_system_count(de_system *const);
unsigned de_system_capacity(de_system *const);
void de_system_end(de_system *const);