
#ifndef TFFU_PARAMS_H_
#define TFFU_PARAMS_H_

#include <cstddef>
#include "TFFU/SerComm.h"

#define PARAM_PTR(ID) (((uint8_t*)&AllParams)+PARAMDATA[ID*2+1])
#define PARAM_TYPE(ID) (PARAMDATA[ID*2])

typedef enum {
	RACEMODE_STOP,
	RACEMODE_NEUTRAL,
	RACEMODE_MANUAL,
	RACEMODE_LEARN,
	RACEMODE_RECALL,
	RACEMODE_SIMPLE,
}RaceMode_t;

typedef struct {
	float p;
	float i;
	float d;
} PIDParams_t;

// PARAMS
#define PARAMCOUNT 15
typedef struct {
	int16_t MaxPWM;
	float Accel;
	PIDParams_t PID_Angle;
	PIDParams_t PID_Motors;
	uint8_t RaceMode; // Hold / Manual / Learn / Memory race / Plain race
	uint8_t DriveMode;
	int16_t ManualThrottle;
	int16_t ManualAngle;
	uint8_t MonitoringEnable;
	uint16_t MonitoringMask;
	uint8_t MonitoringInterval;
} AllParams_t;
extern AllParams_t AllParams;

// Types of of variables and memory offsets
const uint8_t PARAMDATA[ PARAMCOUNT*2 ] = {
	VARTYPE_SWORD, offsetof(AllParams_t, MaxPWM),
	VARTYPE_FLOAT, offsetof(AllParams_t, Accel),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Angle.p),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Angle.i),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Angle.d),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Motors.p),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Motors.i),
	VARTYPE_FLOAT, offsetof(AllParams_t, PID_Motors.d),
	VARTYPE_UBYTE, offsetof(AllParams_t, RaceMode),
	VARTYPE_UBYTE, offsetof(AllParams_t, DriveMode),
	VARTYPE_SWORD, offsetof(AllParams_t, ManualThrottle),
	VARTYPE_SWORD, offsetof(AllParams_t, ManualAngle),
	VARTYPE_UBYTE, offsetof(AllParams_t, MonitoringEnable),
	VARTYPE_UWORD, offsetof(AllParams_t, MonitoringMask),
	VARTYPE_UBYTE, offsetof(AllParams_t, MonitoringInterval),
};


const uint8_t MONVAR_VOLTAGE = 0;
const uint8_t MONVAR_OFFSET = 1;
const uint8_t MONVAR_1 = 2;
const uint8_t MONVAR_2 = 3;
const uint8_t MONVAR_3 = 4;
const uint8_t MONVAR_SENSORS = 5;
const uint8_t MONVAR_DISTANCE = 6;
const uint8_t MONVAR_EVENT = 7;
const uint8_t MONVAR_LSPEED = 8;
const uint8_t MONVAR_RSPEED = 9;
const uint8_t MONVARCOUNT = 10;


void Params_SetDefaults(void);

#endif /* TFFU_PARAMS_H_ */
