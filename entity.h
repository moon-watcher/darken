#pragma once

typedef struct deEntity_t
{
    const deState_t *state;
    deDefinition_t *definition;
    
    // Index of this entity in the deManager_t's entities array
    unsigned index;

    // Bytes for casting data & components
    unsigned char data[];
} deEntity_t;

deEntity_t *de_entity_new    ( const deDefinition_t * );
void        de_entity_delete ( deEntity_t *const );
void        de_entity_set_state  ( deEntity_t *const, const deState_t *const );



// void    entity_list_update (list_t *const);

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