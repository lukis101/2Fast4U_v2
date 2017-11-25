
#include "TFFU/PID.h"


static const float PID_TIMESTEP = 0.001f;
//static const float PID_TIMESTEP = (float)INTERVAL_SENSORS / 1000.f;
//static const float PID_SIGSCALE = 255;

void PID::PID_Init()
{
    val.d = 0;
    val.i = 0;
    lastErr = 0;
    //D_HIST_SIZE = param_racemode;
    for( uint8_t i=0; i<D_HIST_SIZE; i++ )
        d_hist[i] = 0;
    d_hist_i = 0;
    unstable = 200;
}

float PID::PID_Update( int16_t error )
{
    int16_t PIDLIMIT = (int16_t)255;//param_pid_limit;

    // Integral (trapezoidal)
    /*if( pid_integ < 0 ) // Decay
        pid_integ += (1.0-AllParams.pid_d_smoothing)*AllParams.PID_Angle.d;
    else
        pid_integ -= (1.0-AllParams.pid_d_smoothing)*AllParams.PID_Angle.d;*/

    val.i += (float)((error+lastErr)/2) *PID_TIMESTEP * params.i; // Apply Ki gain on accumulation for real-time tuning
    if( val.i > PIDLIMIT ) // Clamp I sum
    	val.i = PIDLIMIT;
    else if( val.i < -PIDLIMIT )
    	val.i = -PIDLIMIT;

    // Proportional
    val.p = error * params.p;

    // Derivative (delta/time)
    float deriv = (float)(error - lastErr) /(PID_TIMESTEP*10);// *param_pid_d;

        // 1)Smoothing using short moving average
    d_hist[ d_hist_i++ ] = deriv;
    if( d_hist_i == D_HIST_SIZE )
        d_hist_i = 0;
    deriv = 0;
    for( uint8_t i=0; i<D_HIST_SIZE; i++ )
        deriv += d_hist[i];
    deriv = deriv / D_HIST_SIZE;
        // 2) Smoothing using exponentially decaying moving average
    deriv_avg = AllParams.pid_d_smoothing*deriv + (1.f-AllParams.pid_d_smoothing)*deriv_avg;

    val.d = deriv_avg * params.d;

    // Unsatability factor
    //float unstabl = fabsf(pid_deriv_avg*param_accel);
    //pid_unstable = param_unstable_smoothing*unstabl + (1.f-param_unstable_smoothing)*pid_unstable;

    // Main
    output = (val.p) + (val.d) + (val.i);

    if( output > PIDLIMIT ) // Clamp PID output
    	output = PIDLIMIT;
    else if( output < -PIDLIMIT )
    	output = -PIDLIMIT;

    lastErr = error;
    return output;
}
