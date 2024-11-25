#pragma once

typedef struct de_entity
{
    void *(*handler)();
    void *(*destructor)();
    void *manager;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void de_entity_update(de_entity *const);
int de_entity_delete(de_entity *const);
void de_entity_destructor(de_entity *const, void *(*)());
void de_entity_set(de_entity *const, void *(*)());
void *de_entity_exec(de_entity *const, void *(*)());
