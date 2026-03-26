#pragma once

#include <stdint.h>
#include "entity.h"
#include "../uclist/uclist.h"

typedef struct de_manager
{
    uclist list;
    uint16_t pause_index;
} de_manager;

void de_manager_init(de_manager *, uint16_t);
de_entity *de_manager_new(de_manager *);
void de_manager_update(de_manager *);
void de_manager_pause(de_manager *);
void de_manager_resume(de_manager *);
void de_manager_iterate(de_manager *, void (*)());
void de_manager_iterateAll(de_manager *, void (*)());
de_entity **de_manager_getList(de_manager *);
uint16_t de_manager_getCapacity(de_manager *);
uint16_t de_manager_countAll(de_manager *);
uint16_t de_manager_count(de_manager *);
void de_manager_reset(de_manager *);
void de_manager_end(de_manager *);
