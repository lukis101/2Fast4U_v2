
#include <stdint.h>
#include <cstddef>

#include "TFFU/Params.h"

AllParams_t AllParams;

void Params_SetDefaults()
{
	AllParams.Speed = 220;
	AllParams.Accel = 1;
	AllParams.SensMin = 400;
	AllParams.SensMax = 900;
	AllParams.PID_Angle  = {1.6, 1.0, 0.30};
	AllParams.PID_Motors = {1.0, 1.0, 1.0};
	AllParams.DriveMode = DRIVEMODE_STOP;
	AllParams.ManualThrottle = 0;
	AllParams.ManualAngle = 0;
	AllParams.MonitoringEnable = 0;
	AllParams.MonitoringInterval = 150;
	AllParams.pid_d_smoothing = 0.5;
	AllParams.pid_unst_smoothing = 0.2;
	AllParams.pid_unst = 0.5;
}

// Types of of variables and memory offsets
const uint8_t PARAMDATA[ PARAMCOUNT*2 ] = {
	VARTYPE_SWORD, offsetof(AllParams_t, Speed),
	VARTYPE_FLOAT, offsetof(AllParams_t, Accel),
	VARTYPE_UWORD, offsetof(AllParams_t, SensMin),
	VARTYPE_UWORD, offsetof(AllParams_t, SensMax),
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
	VARTYPE_FLOAT, offsetof(AllParams_t, pid_unst_smoothing),
	VARTYPE_FLOAT, offsetof(AllParams_t, pid_unst),
};
