#pragma once

#include "uplist.h"

void de_libs_common_init(de_libs_uplist *const this, unsigned int size);
int de_libs_common_resize(de_libs_uplist *const this, unsigned int size);
int de_libs_common_find(de_libs_uplist *const this, void *const data);
void de_libs_common_remove(de_libs_uplist *const this, unsigned int index, void (*callback)(void *const));