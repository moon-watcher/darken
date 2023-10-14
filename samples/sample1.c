#include "include.h"

const de_state looper1;
const de_state looper2;

static void looper_enter1(de_entity *const this)
{
    drawText("looper enter  1",1,1);
    waitMs(1000);
}
static void looper_update1(de_entity *const this)
{
    drawText("looper update 1",1,1);
    waitMs(1000);
    de_state_update(this);
    SYS_doVBlankProcess();
    darken_looper(&main_darken, &looper2, 399);
}
static void looper_leave1(de_entity *const this)
{
    drawText("looper leave  1",1,1);
    waitMs(1000);
}

static void looper_enter2(de_entity *const this)
{
    drawText("looper enter  2",1,1);
    waitMs(1000);
}
static void looper_update2(de_entity *const this)
{
    drawText("looper update 2",1,1);
    waitMs(1000);
    de_state_update(this);
    SYS_doVBlankProcess();
    darken_break(&main_darken);
}
static void looper_leave2(de_entity *const this)
{
    drawText("looper leave  2",1,1);
    waitMs(1000);
}

static void state1_enter (de_entity *const);
static void state1_update(de_entity *const);
static void state1_leave (de_entity *const);
static void state2_enter (de_entity *const);
static void state2_update(de_entity *const);
static void state2_leave (de_entity *const);

const de_state looper1 = {&looper_enter1, &looper_update1, &looper_leave1};
const de_state looper2 = {&looper_enter2, &looper_update2, &looper_leave2};
const de_state state1  = {&state1_enter,  &state1_update,  &state1_leave };
const de_state state2  = {&state2_enter,  &state2_update,  &state2_leave };

static void state1_enter (de_entity *const this) { drawText("state 1 enter ",1,3); waitMs(1000); }
static void state1_update(de_entity *const this) { drawText("state 1 update",1,3); waitMs(1000); de_state_set(this,&state2); }
static void state1_leave (de_entity *const this) { drawText("state 1 leave ",1,3); waitMs(1000); }

static void state2_enter (de_entity *const this) { drawText("state 2 enter ",1,3); waitMs(1000); }
static void state2_update(de_entity *const this) { drawText("state 2 update",1,3); waitMs(1000); }
static void state2_leave (de_entity *const this) { drawText("state 2 leave ",1,3); waitMs(1000); } // waitMs(1000);




void sample1main()
{
    darken_init(&main_darken);

    darken_looper(&main_darken, &looper1, 0);
    darken_state(&main_darken, &state1, 123);

    darken_loop(&main_darken);
   
    drawText("FIN",1,5);
}