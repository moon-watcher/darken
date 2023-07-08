#pragma once

#define dehData(T) \
    T *const data = (T *const) &this->data

#define dehCast(T, V1) \
    T *const V1 = (T *const) &data->V1

#define dehDataAs(T, E, V1) \
    T *const V1 = (T *const) &E->data

#define dehCastAs(T, V1, DATA, V2) \
    T *const V2 = (T *const) &DATA->V1


#define deh_define_state_function(NAME, TYPE, CODE) \
    static void NAME (deEntity_t *const this) { dehData(TYPE); CODE }


typedef deState_t   State;
typedef deEntity_t  Entity;
typedef deSystem_t  System;
typedef deManager_t Manager;

typedef deState_t   *const StateRef;
typedef deEntity_t  *const EntityRef;
typedef deSystem_t  *const SystemRef;
typedef deManager_t *const ManagerRef;