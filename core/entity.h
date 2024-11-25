#pragma once

typedef struct de_entity
{
    void *(*handler)();
    void *(*destructor)();
    void *manager;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

int de_entity_delete(de_entity *const);
