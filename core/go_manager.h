#pragma once

#include "manager.h"

typedef struct go_manager
{
    de_manager manager;
    void *data;
    int pause;
} go_manager;

#define go_manager_init(GOM, BYTES) de_manager_init(&GOM.manager, BYTES); GOM.data = GOM.pause = 0
#define go_manager_new(GOM) de_manager_new(&GOM.manager)
#define go_manager_delete(GOM, ENTITY) de_manager_delete(&GOM.manager, ENTITY)
#define go_manager_reset(GOM) de_manager_reset(&GOM.manager)
#define go_manager_end(GOM) de_manager_end(&GOM.manager)
#define go_manager_pause(GOM) this.pause = 1
#define go_manager_resume(GOM) this.pause = 0
#define go_manager_update(GOM) if (GOM.pause == 0) de_manager_update(&GOM.manager)
