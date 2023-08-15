#pragma once

#include "uplist.h"

void de_libs_common_init(de_libs_uplist *const, unsigned int);
int de_libs_common_resize(de_libs_uplist *const, unsigned int);
int de_libs_common_find(de_libs_uplist *const, void *const);
void de_libs_common_remove(de_libs_uplist *const, unsigned int, void (*)(void *const));