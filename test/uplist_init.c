#include "include.h"

static int y = 2;
static int freemen, usedmem;
static de_entity e1, e2, e3, e4;
static uplist u;
static enum { INIT, ADD, RESET, REMOVE, END };

static de_entity *draw(char i, uplist *u, de_entity *e, char *s)
{
     de_entity *r = NULL;

     switch (i)
     {
          case 0:
               drawText("init", 0, y);
               uplist_init(u);
               break;
          case 1:
               drawText("add", 0, y);
               r = uplist_add(u, e);
               break;
          case 2:
               drawText("reset", 0, y);
               uplist_reset(u);
               break;
          case 3:
               drawText("remove", 0, y);
               uplist_remove(u, e);
               break;
          case 4:
               drawText("end", 0, y);
               uplist_end(u);
               break;
     }

     drawUInt(u->count, 7, y, 2);
     drawText("/", 9, y);
     drawUInt(u->capacity, 10, y, 2);

     drawInt(MEM_getAllocated() - usedmem, 13, y, 4);
     drawInt(freemen - MEM_getFree(),      19, y, 4);
     drawText(s, 25, y);
     
     usedmem = MEM_getAllocated();
     freemen = MEM_getFree();

     ++y;

     return r;
}

void test_uplist_init ()
{
     usedmem = MEM_getAllocated();
     freemen = MEM_getFree();

     drawText("ACTION ITEMS FREE  ALLOC MSG", 0, 0);
          
     draw(INIT,   &u,   0, "");
     draw(ADD,    &u, &e1, "");
     draw(ADD,    &u, &e2, "");
     draw(RESET,  &u,   0, "");
     draw(ADD,    &u, &e1, "");
     draw(ADD,    &u, &e2, "");
     draw(ADD,    &u, &e3, "");
     draw(REMOVE, &u, &e2, "");
     draw(ADD,    &u, &e4, "");
     draw(REMOVE, &u, &e1, "");
     draw(END,    &u,   0, "");
     draw(ADD,    &u, &e1, "");
     draw(ADD,    &u, &e2, "");
     draw(REMOVE, &u, &e3, "not found");
     draw(REMOVE, &u, &e1, "");
     draw(REMOVE, &u, &e2, "");
     draw(ADD,    &u, &e2, "");
     draw(ADD,    &u, &e2, "");
     draw(END,    &u,   0, "");   
}