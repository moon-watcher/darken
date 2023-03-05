#include "darken.h"

void darken(const Entitydef *ed)
{
    manager_init(ed->manager, &(Managerdef){.maxEntities = 1});
    Entity *const e = entity_new(ed);

    while (1)
        e->state->update(e);
}