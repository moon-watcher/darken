#pragma once

#include "../libs/culist.h"
#include "../config/darken.h"

typedef struct de_state de_state;
typedef struct de_entity de_entity;

typedef struct de_manager
{
    culist cul;
    void *data;
} de_manager;

void de_manager_init(de_manager *const, unsigned int);
void de_manager_end(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_update(de_manager *const);
void de_manager_iterate(de_manager *const, void (*)());
de_entity *de_manager_entity_new(de_manager *const, const de_state *const);
int de_manager_entity_delete(de_manager *const, de_entity *const);