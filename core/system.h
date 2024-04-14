#pragma once

#include "../libs/uplist.h"

typedef void (*de_system_f)();

typedef struct de_system
{
    de_system_f update;
    unsigned params;
    uplist list;
} de_system;

void de_system_init(de_system *const, de_system_f, unsigned);
void *de_system_add(de_system *const, void *const);
unsigned de_system_delete(de_system *const, void *const);
void de_system_addEx(de_system *const, ...);
void de_system_deleteEx(de_system *const, void *const); 
void de_system_update(de_system *const);
void de_system_end(de_system *const);

