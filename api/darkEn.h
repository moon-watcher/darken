#pragma once

#include "../darken.h"

typedef struct {
    int  ( *start ) ( const deState_t *const );
    void ( *end   ) ( int );
} DarkenAPI;

const DarkenAPI *const darkEn;
