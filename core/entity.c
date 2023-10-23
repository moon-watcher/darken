#include "state.h"
#include "entity.h"
#include "manager.h"

unsigned int de_entity_set(de_entity *const this, const de_state *const newstate)
{
    if (newstate == 0)
        return 0;

    if (this->state != 0)
        de_entity_leave(this);

    this->state = (de_state *)newstate;
    this->update = this->xtor->update ?: this->state->update ?: ({ void f() {}; f; });

    de_entity_enter(this);

    return 1;
}

__attribute__((always_inline)) inline void de_entity_enter(de_entity *const this)
{
    if (this->state->enter != 0)
        this->state->enter(this);
}

__attribute__((always_inline)) inline void de_entity_update(de_entity *const this)
{
    this->update(this);
}

__attribute__((always_inline)) inline void de_entity_leave(de_entity *const this)
{
    if (this->state->leave != 0)
        this->state->leave(this);
}

__attribute__((always_inline)) inline unsigned int de_entity_delete(de_entity *const this)
{
    return de_manager_entity_delete(this->manager, this);
}

void de_entity_destruct(de_entity *const this)
{
    if (this->state != 0)
        de_entity_leave(this);

    if (this->xtor->leave != 0 && this->xtor->leave != this->state->leave)
        this->xtor->leave(this);
}