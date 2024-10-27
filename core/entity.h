#pragma once

typedef struct
{
    void (*state)();
    void (*destructor)();
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void *de_entity_set(de_entity *const, void (*)());
void *de_entity_destructor(de_entity *const, void (*)());
void *de_entity_exec(de_entity *const, void (*)());
