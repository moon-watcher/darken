#pragma once

typedef struct
{
    void *(*state)();
    void *(*destructor)();
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void *de_entity_null();
void *de_entity_update(de_entity *const);
void *de_entity_setState(de_entity *const, void *(*)());
void *de_entity_setDestructor(de_entity *const, void *(*)());
void *de_entity_exec(de_entity *const, void *(*)());
void *de_entity_destroy(de_entity *const);