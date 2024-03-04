#pragma once

#include "culist.h"
#include "uplist.h"

void upiterator(void **const, unsigned *const, void (*)(), unsigned);
void upiterator_uplist(uplist *const, void (*)(), unsigned);
void upiterator_culist(culist *const, void (*)(), unsigned);