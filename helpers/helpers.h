#pragma once

#define DATA(T) \
    T *const data = (T *const) &this->data

#define REF(T, N, V) \
    T *const V = (T *const) &data->N
    
#define COMP(T, V) \
    T *const V = (T *const) &data->V
    