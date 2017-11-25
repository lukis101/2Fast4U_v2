
#ifndef TFFU_PARAMS_H_
#define TFFU_PARAMS_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "TFFU/SerComm.h"

#define PARAM_PTR(ID) (((uint8_t*)&AllParams)+PARAMDATA[ID*2+1])
#define PARAM_TYPE(ID) (PARAMDATA[ID*2])

typedef struct {
 	float p;
 	float i;
 	float d;
} PIDParams_t;

typedef enum {
	DRIVEMODE_STOP = 0,
	DRIVEMODE_MANUAL,
	DRIVEMODE_STANDBY,
	DRIVEMODE_SIMPLE,
	DRIVEMODE_LEARN,
	DRIVEMODE_RECALL,
} DriveMode_t;

// PARAMS
typedef enum {
	PARAMID_SPEED = 0,
	PARAMID_ACCEL,
	PARAMID_SENS_THRESHOLD,
	PARAMID_PID_ANG_P,
	PARAMID_PID_ANG_I,
	PARAMID_PID_ANG_D,
	PARAMID_PID_MOT_P,
	PARAMID_PID_MOT_I,
	PARAMID_PID_MOT_D,
	PARAMID_DRIVEMODE,
	PARAMID_MANU_THROTTLE,
	PARAMID_MANU_ANGLE,
	PARAMID_MONI_ENABLE,
	PARAMID_MONI_INTERVAL,
	PARAMID_PID_DSMOOTH,
	PARAMCOUNT,
} ParamID_t;

typedef struct {
	int16_t Speed;
	float Accel;
	uint16_t SensThreshold;
	PIDParams_t PID_Angle;
	PIDParams_t PID_Motors;
	uint8_t DriveMode; // Hold / Manual / Learn / Memory race / Plain race
	int16_t ManualThrottle;
	int16_t ManualAngle;
	uint8_t MonitoringEnable;
	uint16_t MonitoringInterval;
	uint16_t pid_d_smoothing;
} AllParams_t;
extern AllParams_t AllParams;

// Types of of variables and memory offsets
extern const uint8_t PARAMDATA[ PARAMCOUNT*2 ];

typedef enum {
	MONVAR_VOLTAGE = 0,
	MONVAR_OFFSET,
	MONVAR_1,
	MONVAR_2,
	MONVAR_3,
	MONVAR_SENSORS,
	MONVAR_LSPEED,
	MONVAR_RSPEED,
}Monvars_t;


void Params_SetDefaults(void);

#ifdef __cplusplus
}
#endif
#endif /* TFFU_PARAMS_H_ */
