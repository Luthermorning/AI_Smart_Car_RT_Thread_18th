#include "placeLoop.h"
#include "pid_all.h"

PositionalPIDpara PlaceXpara;
PositionalPIDpara PlaceYpara;

void placePIDx_Init()
{
    int i;

    PlaceXpara.Kp = 0.25;
    PlaceXpara.Ki = 0.001;
    PlaceXpara.Kd = 0.3;

    PlaceXpara.Deriv = 0;
    PlaceXpara.Err = 0;
    PlaceXpara.Integ = 0;
    PlaceXpara.LastErr = 0;
    PlaceXpara.out = 0;
    PlaceXpara.out_D = 0;
    PlaceXpara.out_I = 0;
    PlaceXpara.out_P = 0;
    PlaceXpara.num_integral = 14;
    PlaceXpara.list_index = 0;
    for (i = 0; i < PlaceXpara.num_integral; i++)
        PlaceXpara.list_value[i] = 0;
}

void placePIDy_Init()
{
    int i;

    PlaceYpara.Kp = 0.3;
    PlaceYpara.Ki = 0.001;
    PlaceYpara.Kd = 0.3;

    PlaceYpara.Deriv = 0;
    PlaceYpara.Err = 0;
    PlaceYpara.Integ = 0;
    PlaceYpara.LastErr = 0;
    PlaceYpara.out = 0;
    PlaceYpara.out_D = 0;
    PlaceYpara.out_I = 0;
    PlaceYpara.out_P = 0;
    PlaceYpara.num_integral = 14;
    PlaceYpara.list_index = 0;
    for (i = 0; i < PlaceYpara.num_integral; i++)
        PlaceYpara.list_value[i] = 0;
}
