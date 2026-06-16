#pragma once

typedef void *(*de_state)(void *);

#define de_state_delete ((de_state)0)
#define de_state_loop ((de_state)1)
#define de_state_pause ((de_state)2)

#define de_state_is_active(STATE) ((STATE) > de_state_pause)
#define de_state_is_deleted(STATE) ((STATE) == de_state_delete)
#define de_state_is_paused(STATE) ((STATE) == de_state_pause)
#define de_state_is_loop(STATE) ((STATE) == de_state_loop)
