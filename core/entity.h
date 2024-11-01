#pragma once

typedef struct
{
    void (*handler)();
    void (*destructor)();
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void de_entity_setState(de_entity *const, void (*)());
void de_entity_setDestructor(de_entity *const, void (*)());
