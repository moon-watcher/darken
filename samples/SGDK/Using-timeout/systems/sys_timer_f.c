#include "include.h"

#include "components/Timer.h"

void sys_timer_f(ComponentTimer *const timer)
{
    ++timer->counter;

    if (timer->counter > timer->max && timer->trigger != 0)
        timer->trigger(timer->param1, timer->param2, timer->param3);
}