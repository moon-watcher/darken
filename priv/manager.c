#include "../debug.h"
#include "../core/manager.h"
#include "../core/entity.h"
#include "manager.h"
#include "entity.h"
#include "../priv/common.h"
#include "../priv/entity.h"
#include "../config.h"

void _de_manager_delete(de_manager *const this, de_entity *const entity)
{
    int ret = uclist_remove(&this->list, entity, 0);

    if (ret == -1)
    {
        DARKEN_DEBUG_WARNING("manager, entity not found");
    }
    else if (ret == -2)
    {
        DARKEN_DEBUG_WARNING("manager, this->count is 0");
    }
}
