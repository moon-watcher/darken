#pragma once

#define deh_data(ENTITY, TYPE, DATA) \
    TYPE *const DATA = (TYPE *const)&ENTITY->data

#define deh_statedata(ENTITY, TYPE, STATEDATA)    \
    if (ENTITY->statedata == 0)                   \
        ENTITY->statedata = malloc(sizeof(TYPE)); \
    TYPE *const STATEDATA = (TYPE *const)ENTITY->statedata;

#define deh_cast(DATA, TYPE, VAR) \
    TYPE *const VAR = (TYPE *const)&DATA->VAR

#define deh_castas(DATA, TYPE, VAR, AS) \
    TYPE *const AS = (TYPE *const)&DATA->VAR
