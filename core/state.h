#pragma once

typedef struct deEntity_t deEntity_t;

typedef void (*deState_f)(deEntity_t *);

typedef struct deState_t
{
    deState_f enter;
    deState_f update;
    deState_f leave;
} deState_t;