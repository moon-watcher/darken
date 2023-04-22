#pragma once

#define dehData(T) \
    T *const data = (T *const) &this->data

#define REF(T, N, V) \
    T *const V = (T *const) &data->N
    
#define dehDataComp(T, V) \
    T *const V = (T *const) &data->V
    