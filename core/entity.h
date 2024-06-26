#pragma once

enum
{
    DARKEN_UPDATEPOLICY_STATE,
    DARKEN_UPDATEPOLICY_STATEXTOR,
    DARKEN_UPDATEPOLICY_XTORSTATE,
    DARKEN_UPDATEPOLICY_XTOR,
};

typedef struct de_state
{
    void (*enter)();
    void (*update)();
    void (*leave)();
} de_state;

typedef struct de_entity
{
    void (*update)();
    de_state *state;
    de_state *xtor;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_entity *de_entity_set(de_entity *const, de_state *);
void de_entity_setPolicy(de_entity *const, unsigned);
void de_entity_updateState(de_entity *const);