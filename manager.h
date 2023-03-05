#pragma once

typedef struct _manager
{
    Entity **entityList;

    // Next free position in entities array
    unsigned free_pos;
    
    // Number of allocated entities
    unsigned allocated_entities;

    // Manager definition
    const Managerdef *definition;

    // unsigned maxEntities;
    // unsigned entities_max_bytes;
} Manager;

void manager_init    ( Manager *const, const Managerdef * );
void manager_destroy ( Manager *const );
void manager_update  ( Manager *const );