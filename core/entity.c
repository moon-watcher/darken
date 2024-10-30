#include "entity.h"
#include "../debug.h"

void *de_entity_null()
{
    DARKEN_WARNING("Entity: exec de_entity_null()");

    return de_entity_null;
}

inline __attribute__((always_inline)) void *de_entity_update(de_entity *const this)
{
    return this->handler = this->handler(this, this->data);
}

void *de_entity_set(de_entity *const this, void *(*state)())
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: set null state");
    }

    return this->handler = state ?: de_entity_null;
}

void *de_entity_exec(de_entity *const this, void *(*state)())
{
    if (state == 0)
    {
        DARKEN_ERROR("Entity: exec null state");
    }

    return this->handler = state ? exec(this, this->data) : de_entity_null;
}

void *de_entity_destructor(de_entity *const this, void *(*state)())
{
    return this->destructor = state ?: de_entity_null;
}

void *de_entity_destroy(de_entity *const this)
{
    this->handler = this->destructor ? this->destructor(this, this->data) : de_entity_null;
    this->destructor = de_entity_null;

    return this->handler;
}
