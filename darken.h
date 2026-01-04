#pragma once

typedef void *(*de_state)(void *);

typedef struct
{
    de_state state;
    de_state destructor;
    unsigned char data[];
} de_entity;

#include "config.h"
typedef uclist de_manager;

void darken_init(de_manager *, unsigned short);
de_entity *darken_new(de_manager *, de_state);
void darken_update(de_manager *);
void darken_reset(de_manager *);
void darken_end(de_manager *);

//

void de_entity_setState(de_entity *, de_state);
de_state de_entity_getState(de_entity *);
void de_entity_pause(de_entity *);
void de_entity_delete(de_entity *);
