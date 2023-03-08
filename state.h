#pragma once

typedef struct deState_t
{
    char *name;
    void (*enter) (deEntity_t *const);
    void (*update)(deEntity_t *const);
    void (*leave) (deEntity_t *const);
} deState_t;
