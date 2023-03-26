#pragma once

typedef struct {
    void ( *init   ) ( deSystem_t *const, const deDefinition_t * );
    void ( *update ) ( deSystem_t *const );
    void ( *end    ) ( deSystem_t *const );
    void ( *add    ) ( deSystem_t *const, void *const[]);
} deSystemAPI;

typedef struct {
    void ( *init   ) ( deManager_t *const, const deDefinition_t * );
    void ( *update ) ( deManager_t *const );
    void ( *end    ) ( deManager_t *const );
} deManagerAPI;

typedef struct {
    deEntity_t   *( *new    ) ( const deDefinition_t * );
    void  ( *state  ) ( deEntity_t *const, const deState_t *const );
    void  ( *delete ) ( deEntity_t *const );
} deEntityAPI;

typedef struct {
    int  ( *init ) ( const deDefinition_t * );
    void ( *end  ) ( int );
} DarkenAPI;

const DarkenAPI    *const darkEn;
const deSystemAPI  *const deSystem;
const deManagerAPI *const deManager;
const deEntityAPI  *const deEntity;
