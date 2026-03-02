#pragma once

typedef void *(*de_state)(void *);

#define DE_STATE_DELETE ((void *)0)
#define DE_STATE_LOOP ((void *)1)
#define DE_STATE_PAUSE ((void *)2)

#define DE_STATE_IS_ACTIVE(state) ((state) > (de_state)2)
#define DE_STATE_IS_DELETED(state) ((state) == (de_state)0)
#define DE_STATE_IS_PAUSED(state) ((state) == (de_state)2)
#define DE_STATE_NEED_UPDATE(state) ((state) != (de_state)1)
