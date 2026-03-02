#pragma once

#include <stdint.h>
#include "list.h"
#include "entity.h"

typedef struct de_manager
{
    de_list list;
    uint16_t pause_index;
    //
    // char *name;
} de_manager;

void de_manager_init(de_manager *, uint16_t);
de_entity *de_manager_new(de_manager *);
void de_manager_update(de_manager *);
void de_manager_pause(de_manager *);
void de_manager_resume(de_manager *);
void de_manager_iterate(de_manager *, void (*)());
void de_manager_iterateAll(de_manager *, void (*)());
void de_manager_reset(de_manager *);
void de_manager_end(de_manager *);
