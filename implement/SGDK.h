#pragma once

#include <genesis.h>

#ifdef DARKEN_DEBUG
#undef DARKEN_DEBUG
#define DARKEN_DEBUG 1
#endif

/*
#ifdef DARKEN_INFO
#undef DARKEN_INFO
#define DARKEN_INFO(M) kprintf("Darken INFO: %s", M)
#endif

#ifdef DARKEN_ERROR
#undef DARKEN_ERROR
#define DARKEN_ERROR(M) kprintf("Darken ERROR: %s", M)
#endif

#ifdef DARKEN_NOTICE
#undef DARKEN_NOTICE
#define DARKEN_NOTICE(M) kprintf("Darken NOTICE: %s", M)
#endif

#ifdef DARKEN_WARNING
#undef DARKEN_WARNING
#define DARKEN_WARNING(M) kprintf("Darken WARNING: %s", M)
#endif
*/