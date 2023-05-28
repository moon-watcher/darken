#pragma once

#define dehData(T) \
    T *const data = (T *const) &this->data

#define dehDataAs(T, ENTITY) \
    T *const data = (T *const) &ENTITY->data

#define dehDataCompAs(T, V, N) \
    T *const V = (T *const) &data->N
    
#define dehDataComp(T, V) \
    T *const V = (T *const) &data->V
    


// for lazzyes like me

typedef deState_t   Scene;
typedef deState_t   State;
typedef deEntity_t  Entity;
typedef deSystem_t  System;
typedef deManager_t Manager;

typedef deState_t   *const SceneRef;
typedef deState_t   *const StateRef;
typedef deEntity_t  *const EntityRef;
typedef deSystem_t  *const SystemRef;
typedef deManager_t *const ManagerRef;
