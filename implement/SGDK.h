#pragma once

#include <genesis.h>

#ifdef _DARKEN_DEBUG
#undef _DARKEN_DEBUG
#define _DARKEN_DEBUG 1
#endif

/*
#ifdef _DARKEN_DEBUG_ERROR
#undef _DARKEN_DEBUG_ERROR
#define _DARKEN_DEBUG_ERROR(M) kprintf("Darken ERROR: %s", M)
#endif

#ifdef _DARKEN_DEBUG_NOTICE
#undef _DARKEN_DEBUG_NOTICE
#define _DARKEN_DEBUG_NOTICE(M) kprintf("Darken NOTICE: %s", M)
#endif

#ifdef _DARKEN_DEBUG_WARNING
#undef _DARKEN_DEBUG_WARNING
#define _DARKEN_DEBUG_WARNING(M) kprintf("Darken WARNING: %s", M)
#endif
*/