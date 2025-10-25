#include "planLoop.h"
#include "pid_all.h"

PositionalPIDpara PlanXpara;
PositionalPIDpara PlanYpara;

void planPIDx_Init()
{
    int i;

    PlanXpara.Kp = 1.2;
    PlanXpara.Ki = 0.6;
		PlanXpara.Kii = 0.3;
    PlanXpara.Kd = 600;

    PlanXpara.Deriv = 0;
    PlanXpara.Err = 0;
    PlanXpara.Integ = 0;
    PlanXpara.LastErr = 0;
    PlanXpara.out = 0;
    PlanXpara.out_D = 0;
    PlanXpara.out_I = 0;
    PlanXpara.out_P = 0;
    PlanXpara.num_integral = 14;
    PlanXpara.list_index = 0;
    for (i = 0; i < PlanXpara.num_integral; i++)
        PlanXpara.list_value[i] = 0;
}

void planPIDy_Init()
{
    int i;

    PlanYpara.Kp = 1.2;
    PlanYpara.Ki = 0.6;
		PlanYpara.Kii = 0.3;
    PlanYpara.Kd = 600;

    PlanYpara.Deriv = 0;
    PlanYpara.Err = 0;
    PlanYpara.Integ = 0;
    PlanYpara.LastErr = 0;
    PlanYpara.out = 0;
    PlanYpara.out_D = 0;
    PlanYpara.out_I = 0;
    PlanYpara.out_P = 0;
    PlanYpara.num_integral = 14;
    PlanYpara.list_index = 0;
    for (i = 0; i < PlanYpara.num_integral; i++)
        PlanYpara.list_value[i] = 0;
}
