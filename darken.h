#pragma once

#include <stdint.h>

typedef void *(*de_state)(void *);

typedef struct
{
    de_state state;
    de_state destructor;
    uint8_t data[];
} de_entity;

#include "config.h"
typedef uclist de_manager;

void darken_init(de_manager *, uint16_t);
de_entity *darken_new(de_manager *, de_state);
void darken_update(de_manager *);
void darken_reset(de_manager *);
void darken_end(de_manager *);

#define DE_DELETE 0
#define DE_LOOP 1
#define DE_PAUSE 2
