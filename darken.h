#pragma once

typedef struct deEntity_t     deEntity_t;
typedef struct deManager_t    deManager_t;
typedef struct deState_t      deState_t;
typedef struct deSystem_t     deSystem_t;
typedef struct deDefinition_t deDefinition_t;

#include "definition.h"
#include "entity.h"
#include "manager.h"
#include "state.h"
#include "system.h"

int darken(const deState_t *, void (*)(deEntity_t *const));
void darken_end(int);

// long data2;
// void *ptr2;
// void (*fptr2)();

// unsigned char damage;
// int life;
// unsigned char chances;

// Vect2D_s32 center;

// s8 directionX;
// s32 tempo;
// unsigned char attack;
// u16 attackTime;
// unsigned char brave;
// s8 jump;
// unsigned char fallingdown;
// unsigned char fallingdownB;
// unsigned char damageTime;
// unsigned char lose;
// unsigned char loseTime;
// u16 won;
// unsigned char flip;
// unsigned char stairs;

// fix32 velX, velY;
// fix32 acelX, acelY;
// fix32 posX, posY;

// fix32 posXi, posYi;
// fix32 posXr, posYr;
// fix32 posXf, posYf;

// fix32 distancia;
// unsigned char sprAltura;
// unsigned char type;
// unsigned char monstro;
// fix32 varT;
// fix32 valorT;
// fix32 valorA;

// unsigned char dispara;
// s8 xInc, yInc;
// fix32 dX, dY;
// fix32 eX, eY;
// fix32 eN;
// fix32 eXY;

// int anim;
// const SpriteDefinition *sd;

// enum
// {
//     INACTIVE,
//     ACTIVE,
//     SPECIAL,
// } active;