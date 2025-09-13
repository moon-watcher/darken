#pragma once

#include "entity.h"

typedef struct de_manager
{
    void **items;
    unsigned size;
    unsigned capacity;
    unsigned itemSize;
    unsigned pack;
} de_manager;

void de_manager_init(de_manager *const, unsigned);
de_entity *de_manager_new(de_manager *const, de_state, de_state);
void de_manager_update(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_end(de_manager *const);

//

unsigned de_manager_getSize(de_manager *const);
unsigned de_manager_getCapacity(de_manager *const);
de_entity **de_manager_getItems(de_manager *const);
