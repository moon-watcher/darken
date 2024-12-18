#pragma once

#include "uclist.h"

int uclist_extend_findIndex(uclist *const, void *const);
void uclist_extend_removeIndex(uclist *const, unsigned, void (*)());
int uclist_extend_iterator(uclist *const, void (*)(), unsigned);
