#pragma once

typedef void *(*de_state)(void *);

#define DE_STATE_DELETE ((de_state)0)
#define DE_STATE_LOOP ((de_state)1)
#define DE_STATE_PAUSE ((de_state)2)

#define DE_STATE_IS_ACTIVE(state) ((state) > DE_STATE_PAUSE)
#define DE_STATE_IS_DELETED(state) ((state) == DE_STATE_DELETE)
#define DE_STATE_IS_PAUSED(state) ((state) == DE_STATE_PAUSE)
#define DE_STATE_NEED_UPDATE(state) ((state) != DE_STATE_LOOP)
