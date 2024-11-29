#pragma once

typedef struct de_entity
{
    void *(*handler)();
    void *(*destructor)();
    //
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void *de_entity_update(de_entity *const);
void *de_entity_delete(de_entity *const);
