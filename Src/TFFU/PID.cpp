
#include "TFFU/PID.h"

static const float PID_TIMESTEP = 0.01f;
//static const float PID_TIMESTEP = (float)INTERVAL_SENSORS / 1000.f;
//static const float PID_SIGSCALE = 255;

PID::PID(PIDParams_t* koeffs)
{
	params = koeffs;
	Init();
}

void PID::Init()
{
    values.d = 0;
    values.i = 0;
    lastErr = 0;
    //D_HIST_SIZE = param_racemode;
    for( uint8_t i=0; i<D_HIST_SIZE; i++ )
        d_hist[i] = 0;
    d_hist_i = 0;
    //unstable = 200;
}

float PID::Update( float error )
{
    //int16_t PIDLIMIT = (int16_t)255;//param_pid_limit;
    float PIDLIMIT = 1.0f;

    // Integral (trapezoidal)
    /*if( pid_integ < 0 ) // Decay
        pid_integ += (1.0-AllParams.pid_d_smoothing)*AllParams.PID_Angle.d;
    else
        pid_integ -= (1.0-AllParams.pid_d_smoothing)*AllParams.PID_Angle.d;*/

    values.i += ((error+lastErr)/2) *PID_TIMESTEP * params->i; // Apply Ki gain on accumulation for real-time tuning
    if( values.i > PIDLIMIT ) // Clamp I sum
    	values.i = PIDLIMIT;
    else if( values.i < -PIDLIMIT )
    	values.i = -PIDLIMIT;

    // Proportional
    values.p = error * params->p;

    // Derivative (delta/time)
    float deriv = (error - lastErr) / (PID_TIMESTEP);

        // 1)Smoothing using short moving average
    d_hist[ d_hist_i++ ] = deriv;
    if( d_hist_i == D_HIST_SIZE )
        d_hist_i = 0;
    deriv = 0;
    for( uint8_t i=0; i<D_HIST_SIZE; i++ )
        deriv += d_hist[i];
    deriv = deriv / D_HIST_SIZE;
        // 2) Smoothing using exponentially decaying moving average
    deriv_avg = deriv*AllParams.pid_d_smoothing + deriv_avg*(1.f-AllParams.pid_d_smoothing);

    values.d = deriv_avg * params->d;

    // Unsatability factor
    //float unstabl = deriv_avg*AllParams.pid_unst;
    //unstability = unstabl*AllParams.pid_unst_smoothing + unstability*(1.f-AllParams.pid_unst_smoothing);

    // Main
    float outp = (values.p) + (values.d);// + (values.i);

    if( outp > PIDLIMIT ) // Clamp PID output
    	outp = PIDLIMIT;
    else if( outp < -PIDLIMIT )
    	outp = -PIDLIMIT;

    output = outp;
    lastErr = error;
    return outp;
}
