#pragma once

enum
{
    DARKEN_UPDATEPOLICY_STATE,
    DARKEN_UPDATEPOLICY_STATEXTOR,
    DARKEN_UPDATEPOLICY_XTORSTATE,
    DARKEN_UPDATEPOLICY_XTOR,
};

typedef void (*de_state_f)();

typedef struct de_state
{
    de_state_f enter;
    de_state_f update;
    de_state_f leave;
} de_state;

typedef struct de_entity
{
    // void (*update)();
    de_state *state;
    de_state *newState;
    de_state_f destroy;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_entity *de_entity_set(de_entity *const, de_state *);
// void de_entity_setPolicy(de_entity *const, unsigned);
// void de_entity_updateState(de_entity *const);

#define de_state_enter(ENTITY)  ENTITY->state->enter (ENTITY, ENTITY->data);
#define de_state_update(ENTITY) ENTITY->state->update(ENTITY, ENTITY->data);
#define de_state_leave(ENTITY)  ENTITY->state->leave (ENTITY, ENTITY->data);
