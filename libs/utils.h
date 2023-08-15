#pragma once

#include "uplist.h"

void delutil_init(uplist_t *const this, unsigned int size);
int delutil_resize(uplist_t *const this, unsigned int size);
int delutil_find(uplist_t *const this, void *const data);
void delutil_remove(uplist_t *const this, unsigned int index, void (*callback)(void *const));