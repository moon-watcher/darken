#pragma once

typedef struct deManager_t deManager_t;

typedef struct deEntity_t
{
    deState_t *state;

    deState_f update;
    deState_f destructor;

    deManager_t *manager;
    unsigned char data[]; // Bytes for casting data & components
} deEntity_t;

deEntity_t *deEntity_new(const deState_t *const entity);
void deEntity_delete(deEntity_t *const entity);