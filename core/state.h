#pragma once

typedef struct deEntity_t deEntity_t;

typedef void (*deState_f) (deEntity_t *);

typedef struct deState_t
{
    deState_f enter;
    deState_f update;
    deState_f leave;
} deState_t;

void deState_exec   ( deEntity_t *const, deState_f );
void deState_enter  ( deEntity_t *const );
void deState_update ( deEntity_t *const );
void deState_leave  ( deEntity_t *const );
