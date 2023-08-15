#pragma once

typedef struct deEntity_t deEntity_t;

typedef void (*deState_f)(deEntity_t *);

typedef struct deState_t
{
    deState_f enter;
    deState_f update;
    deState_f leave;
} deState_t;

void deState_set(deEntity_t *const entity, const deState_t *const state);
void deState_force(deEntity_t *const entity, const deState_t *const state);
void deState_update(deEntity_t *const entity);
void deState_destruct(deEntity_t *const entity);