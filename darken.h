#pragma once

typedef struct _entity     Entity;
typedef struct _entitydef  Entitydef;
typedef struct _manager    Manager;
typedef struct _managerdef Managerdef;
typedef struct _state      State;
typedef struct _system     System;
typedef struct _systemdef  Systemdef;

#include "entity.h"
#include "entitydef.h"
#include "manager.h"
#include "managerdef.h"
#include "state.h"
#include "system.h"
#include "systemdef.h"

int  darken     ( const State * );
void darken_end ( int );


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