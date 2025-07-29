#include "state.h"

inline void *de_state_empty()
{
    return de_state_empty;
}

inline void *de_state_exec(de_state this, de_entity *const entity)
{
    return this(entity->data, entity);
}

inline void *de_state_update(de_state this, de_entity *const entity)
{
    return entity->state = de_state_exec(this, entity);
}
