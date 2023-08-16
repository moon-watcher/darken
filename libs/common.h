#pragma once

#include "uplist.h"

void de_common_init(uplist *const, unsigned int);
int de_common_resize(uplist *const, unsigned int);
int de_common_find(uplist *const, void *const);
void de_common_remove(uplist *const, unsigned int, void (*)(void *const));