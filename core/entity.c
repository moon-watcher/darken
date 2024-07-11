#include "entity.h"

static void _nullf();
static const de_state _empty_state = {_nullf, _nullf, _nullf};

//

void de_entity_set(de_entity *const this, de_state *const state)
{
    this->state = state ?: &_empty_state;
    this->ctrl = 3;
}

void de_entity_delay(de_entity *const this)
{
    this->ctrl = 1;
}

void de_entity_delete(de_entity *const this)
{
    this->ctrl = 2;
}

//

static void _nullf()
{
    //
}