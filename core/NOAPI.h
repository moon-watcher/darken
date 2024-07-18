#pragma once

#include "entity.h"

extern const void (*const NOAPI_entity_array[])(de_entity *const);

void NOAPI_entity_destroy(de_entity *const);
