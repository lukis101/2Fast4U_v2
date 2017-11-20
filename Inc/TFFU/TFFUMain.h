
#ifndef TFFUMAIN_H_
#define TFFUMAIN_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
 extern "C" {
#endif


// VARS
extern uint8_t raceActive;
extern uint8_t standby;

// INTERNALS
enum EVENTS
{
	EVENT_STRAIGHT,
	EVENT_CORNER_LEFT,
	EVENT_CORNER_RIGHT,
	EVENT_CURVE_LEFT,
	EVENT_CURVE_RIGHT,
	EVENT_CROSS,
	EVENT_DASH,
	EVENT_END,
	EVENT_BRICK,
	EVENT_RAMP,
	EVENT_CIRCLE,
};
extern const uint8_t NUMEVENTS;
//uint8_t events[NUMEVENTS] = {};
extern uint8_t lastevent;

extern const uint16_t INTERVAL_VOLTAGE;
extern const uint16_t INTERVAL_SENSORS;
extern const uint16_t INTERVAL_MOTORS;
extern const uint16_t INTERVAL_PID;
extern uint32_t lastTick_voltage;
extern uint32_t lastTick_monitor;
extern uint32_t nextTick_monitor;
extern uint32_t lastTick_motors;
extern uint32_t lastTick_sensors;
extern uint32_t nextTick_sensors;
extern bool startFlag;
extern bool stopFlag;


void TFFUMain(void);


#ifdef __cplusplus
 }
#endif

#endif /* TFFUMAIN_H_ */
