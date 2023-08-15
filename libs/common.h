#pragma once

#include "uplist.h"

void darkenLibsCommon_init(uplist_t *const this, unsigned int size);
int darkenLibsCommon_resize(uplist_t *const this, unsigned int size);
int darkenLibsCommon_find(uplist_t *const this, void *const data);
void darkenLibsCommon_remove(uplist_t *const this, unsigned int index, void (*callback)(void *const));