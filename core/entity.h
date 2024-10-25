#pragma once

typedef struct
{
    void (*state)();
    void (*destructor)();
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_entity *de_entity_set(de_entity *const, void (*[static 1])());
de_entity *de_entity_destructor(de_entity *const, void (*)());
