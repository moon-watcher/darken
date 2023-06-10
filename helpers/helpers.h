#pragma once

#define dehData(T) \
    T *const data = (T *const) &this->data

#define dehDataAs(T, E) \
    T *const data = (T *const) &E->data

#define dehDataComp(T, V) \
    T *const V = (T *const) &data->V

#define dehDataCompAs(T, V, N) \
    T *const V = (T *const) &data->N

#define dehCast(S, T, V) \
    T *const V = ((T *const) &((S *const) &this->data)->V)

#define dehCastAs(S, T, V, N) \
    T *const V = ((T *const) &((S *const) &this->data)->N)

#define deh_define_state_function(NAME, TYPE, CODE ) \
    static void NAME (deEntity_t *const this) { dehData(TYPE); CODE  }

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
