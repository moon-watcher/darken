#include "entity.h"

const de_state de_state_empty = {de_state_null, de_state_null, de_state_null};

__attribute__((always_inline)) inline void de_state_update(de_entity *const entity)
{
    de_state_exec(entity, update);
}

__attribute__((always_inline)) inline void de_state_null(de_entity *const entity)
{
    // :)
}


// 
// Privadas de Darken. Sacar de aquí
// 
__attribute__((always_inline)) inline void de_state_enter(de_entity *const entity)
{
    de_state_exec(entity, enter);
}

__attribute__((always_inline)) inline void de_state_leave(de_entity *const entity)
{
    de_state_exec(entity, leave);
    entity->state = 0;

#if DARKEN_ENTITY_TEMPDATA
#include "../config/free.h"
    free(entity->tempdata);
    entity->tempdata = 0;
#endif
}

__attribute__((always_inline)) inline void de_xtor_enter(de_entity *const entity)
{
    if (entity->xtor->enter != 0)
        entity->xtor->enter(entity);
}

__attribute__((always_inline)) inline void de_xtor_leave(de_entity *const entity)
{
    if (entity->xtor->leave != 0 /* && entity->xtor->leave != entity->state->leave*/)
        entity->xtor->leave(entity);

    entity->xtor = 0;
}
//
//
//