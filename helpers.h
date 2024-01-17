#pragma once

#include "config.h"

#if DARKEN_ENTITY_DATA
#define deh_data(ENTITY, TYPE, DATA) \
    TYPE *const DATA = (TYPE *const)&ENTITY->data
#else
#define deh_data
#endif

#if DARKEN_ENTITY_TEMPDATA
#define deh_tempdata(ENTITY, TYPE, TEMPDATA)     \
    if (ENTITY->tempdata == 0)                   \
        ENTITY->tempdata = malloc(sizeof(TYPE)); \
    TYPE *const TEMPDATA = (TYPE *const)ENTITY->tempdata;
#else
#define deh_tempdata
#endif

#define deh_cast(DATA, TYPE, VAR) \
    TYPE *const VAR = (TYPE *const)&DATA->VAR

#define deh_castAs(DATA, TYPE, VAR, AS) \
    TYPE *const AS = (TYPE *const)&DATA->VAR
