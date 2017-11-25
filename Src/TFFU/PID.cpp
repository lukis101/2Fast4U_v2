
#include "TFFU/PID.h"
#include "TFFU/Params.h"

float pid_prop;
float pid_integ;
float pid_deriv;
float pid_deriv_avg;
float pid_unstable;
int16_t pid_output;

int16_t lastErr;

static uint8_t D_HIST_SIZE = 4;
static float d_hist[ 100 ];
static uint8_t d_hist_i;

static const float PID_TIMESTEP = 0.001f;
//static const float PID_TIMESTEP = (float)INTERVAL_SENSORS / 1000.f;
//static const float PID_SIGSCALE = 255;

void PID_Init()
{
    pid_deriv = 0;
    pid_integ = 0;
    lastErr = 0;
    //D_HIST_SIZE = param_racemode;
    for( uint8_t i=0; i<D_HIST_SIZE; i++ )
        d_hist[i] = 0;
    d_hist_i = 0;
    pid_unstable = 200;
}

void PID_Update( int16_t error )
{
    int16_t PIDLIMIT = (int16_t)255;//param_pid_limit;

    // Integral (trapezoidal)
    /*if( pid_integ < 0 ) // Decay
        pid_integ += (1.0-AllParams.pid_d_smoothing)*AllParams.PID_Angle.d;
    else
        pid_integ -= (1.0-AllParams.pid_d_smoothing)*AllParams.PID_Angle.d;*/

    pid_integ += (float)((error+lastErr)/2) *PID_TIMESTEP *AllParams.PID_Angle.i; // Apply Ki gain on accumulation for real-time tuning
    if( pid_integ > PIDLIMIT ) // Clamp I sum
        pid_integ = PIDLIMIT;
    else if( pid_integ < -PIDLIMIT )
        pid_integ = -PIDLIMIT;

    // Proportional
    pid_prop = error * AllParams.PID_Angle.d;

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
    pid_deriv_avg = AllParams.pid_d_smoothing*deriv + (1.f-AllParams.pid_d_smoothing)*pid_deriv_avg;

    pid_deriv = pid_deriv_avg *AllParams.PID_Angle.d;

    // Unsatability factor
    //float unstabl = fabsf(pid_deriv_avg*param_accel);
    //pid_unstable = param_unstable_smoothing*unstabl + (1.f-param_unstable_smoothing)*pid_unstable;

    // Main
    int16_t pidout = (pid_prop) + (pid_deriv) + (pid_integ);

    if( pidout > PIDLIMIT ) // Clamp PID output
        pidout = PIDLIMIT;
    else if( pidout < -PIDLIMIT )
        pidout = -PIDLIMIT;

    pid_output = pidout;
    lastErr = error;
}
