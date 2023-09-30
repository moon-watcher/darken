#pragma once

typedef struct de_entity de_entity;
typedef struct de_state de_state;

#include "../libs/culist.h"
#include "../config/darken.h"

typedef struct de_manager
{
    culist cul;
    unsigned char pause : 1;
#if DARKEN_MANAGER_DATA
    void *data;
#endif
} de_manager;

void de_manager_init(de_manager *const, unsigned int, unsigned int);
void de_manager_end(de_manager *const);
void de_manager_reset(de_manager *const);
void de_manager_update(de_manager *const);
void de_manager_iterate(de_manager *const, void (*)());
void *de_manager_data(de_manager *const, unsigned int);
void de_manager_free(de_manager *const);

de_entity *de_manager_entity_create(de_manager *const, const de_state *const);
void de_manager_entity_delete(de_manager *const, de_entity *const);
void de_manager_entity_iterator(de_manager *const, void (*)(de_entity *const));
