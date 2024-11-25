#pragma once

#include "../uclist/uclist.h"

typedef struct de_manager
{
    uclist list;
    void(*update);
    void(*destroy);
} de_manager;

void de_manager_initialize(de_manager *const, unsigned, void(*), void(*));
void de_manager_update(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);
