
#include <stdint.h>
#include <cstddef>

#include "TFFU/Params.h"

AllParams_t AllParams;

void Params_SetDefaults()
{
	AllParams.Speed = 50;
	AllParams.Accel = 1;
	AllParams.SensThreshold = 750;
	AllParams.PID_Angle  = {1.0, 1.0, 1.0};
	AllParams.PID_Angle  = {1.0, 1.0, 1.0};
	AllParams.DriveMode = DRIVEMODE_STOP;
	AllParams.ManualThrottle = 0;
	AllParams.ManualAngle = 0;
	AllParams.MonitoringEnable = 1;
	AllParams.MonitoringInterval = 200;
	AllParams.pid_d_smoothing = 1;;
}

// Types of of variables and memory offsets
const uint8_t PARAMDATA[ PARAMCOUNT*2 ] = {
	VARTYPE_SWORD, offsetof(AllParams_t, Speed),
	VARTYPE_FLOAT, offsetof(AllParams_t, Accel),
	VARTYPE_UWORD, offsetof(AllParams_t, SensThreshold),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Angle.p),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Angle.i),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Angle.d),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Motors.p),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Motors.i),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Motors.d),
	VARTYPE_UBYTE, offsetof(AllParams_t, DriveMode),
	VARTYPE_SWORD, offsetof(AllParams_t, ManualThrottle),
	VARTYPE_SWORD, offsetof(AllParams_t, ManualAngle),
	VARTYPE_UBYTE, offsetof(AllParams_t, MonitoringEnable),
	VARTYPE_UWORD, offsetof(AllParams_t, MonitoringInterval),
	VARTYPE_FLOAT, offsetof(AllParams_t, pid_d_smoothing),
};

// PARAMS
/*int16_t param_maxspeed = 90;
float param_accel = 0.9;
float param_pid_p = 2.5;
float param_pid_i = 0.5;
float param_pid_d = 2.0;
float param_mpid_p = 1;
float param_mpid_i = 1;
float param_mpid_d = 0.1;
uint8_t param_manual_enable = 0;
int16_t param_manual_speed = 0;
int16_t param_manual_angle = 0;
uint8_t param_monitor_enable = 0;
uint16_t param_monitor_interval_1 = 200;
uint8_t param_testmode = 0;
uint8_t param_racemode = 0;
float param_pid_d_smoothing = 0.06;
uint8_t param_sensor_weight = 30;
uint8_t param_pid_limit = 220;
float param_motor_balance = 0.85; // multiplied with left pwm
float param_unstable_smoothing = 0.04;*/
