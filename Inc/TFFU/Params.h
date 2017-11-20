
#ifndef TFFU_PARAMS_H_
#define TFFU_PARAMS_H_

// PARAMS
extern int16_t param_maxspeed = 90;
extern float param_accel = 0.9;
extern float param_pid_p = 2.5;
extern float param_pid_i = 0.5;
extern float param_pid_d = 2.0;
extern float param_mpid_p = 1;
extern float param_mpid_i = 1;
extern float param_mpid_d = 0.1;
extern uint8_t param_manual_enable = 0;
extern int16_t param_manual_speed = 0;
extern int16_t param_manual_angle = 0;
extern uint8_t param_monitor_enable = 0;
extern uint16_t param_monitor_interval_1 = 200;
extern uint8_t param_testmode = 0;
extern uint8_t param_racemode = 0;
extern float param_pid_d_smoothing = 0.06;
extern uint8_t param_sensor_weight = 30;
extern uint8_t param_pid_limit = 220;
extern float param_motor_balance = 0.85; // multiplied with left pwm
extern float param_unstable_smoothing = 0.04;

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

const uint8_t PARAM_MAXSPEED = 60;
const uint8_t PARAM_ACCEL = 1;
const uint8_t PARAM_PID_P = 2;
const uint8_t PARAM_PID_I = 3;
const uint8_t PARAM_PID_D = 4;
const uint8_t PARAM_MPID_P = 5;
const uint8_t PARAM_MPID_I = 6;
const uint8_t PARAM_MPID_D = 7;
const uint8_t PARAM_MANUAL_ENABLE = 8;
const uint8_t PARAM_MANUAL_SPEED = 9;
const uint8_t PARAM_MANUAL_ANGLE = 10;
const uint8_t PARAM_MONITOR_ENABLE = 11;
const uint8_t PARAM_MONITOR_INTERVAL_1 = 12;
const uint8_t PARAM_TESTMODE = 13;
const uint8_t PARAM_RACEMODE = 14;
const uint8_t PARAM_PID_D_SMOOTHING = 15;
const uint8_t PARAM_SENSOR_WEIGHT = 16;
const uint8_t PARAM_PID_LIMIT = 17;
const uint8_t PARAM_MOTOR_BALANCE = 18;
const uint8_t PARAM_UNSTABLE_SMOOTHING = 19;
const uint8_t PARAMCOUNT = 20;

// Types of of variables and eeprom offsets
const uint8_t PARAMDATA[ PARAMCOUNT*2 ] = {
    VARTYPE_SWORD, 1, // PARAM_MAXSPEED
    VARTYPE_FLOAT, 3, // PARAM_ACCEL
    VARTYPE_FLOAT, 7, // PARAM_PID_P
    VARTYPE_FLOAT, 11, // PARAM_PID_I
    VARTYPE_FLOAT, 15, // PARAM_PID_D
    VARTYPE_UWORD, 19, // PARAM_MPID_P
    VARTYPE_UWORD, 23, // PARAM_MPID_I
    VARTYPE_FLOAT, 27, // PARAM_MPID_D
    VARTYPE_UBYTE, 31, // PARAM_MANUAL_ENABLE
    VARTYPE_SWORD, 32, // PARAM_MANUAL_SPEED
    VARTYPE_SWORD, 34, // PARAM_MANUAL_ANGLE
    VARTYPE_UBYTE, 36, // PARAM_MONITOR_ENABLE
    VARTYPE_UWORD, 37, // PARAM_MONITOR_INTERVAL_1
    VARTYPE_UBYTE, 39, // PARAM_TESTMODE
    VARTYPE_UBYTE, 40, // PARAM_RACEMODE
    VARTYPE_FLOAT, 41, // PARAM_PID_D_SMOOTHING
    VARTYPE_UBYTE, 45, // PARAM_SENSOR_WEIGHT
    VARTYPE_UBYTE, 46, // PARAM_PID_LIMIT
    VARTYPE_FLOAT, 47, // PARAM_MOTOR_BALANCE
    VARTYPE_FLOAT, 51, // PARAM_UNSTABLE_SMOOTHING
};
void * const PARAM_PTRS[ PARAMCOUNT ] = { // Const pointers to params
    (void*)&param_maxspeed,
    (void*)&param_accel,
    (void*)&param_pid_p,
    (void*)&param_pid_i,
    (void*)&param_pid_d,
    (void*)&param_mpid_p,
    (void*)&param_mpid_i,
    (void*)&param_mpid_d,
    (void*)&param_manual_enable,
    (void*)&param_manual_speed,
    (void*)&param_manual_angle,
    (void*)&param_monitor_enable,
    (void*)&param_monitor_interval_1,
    (void*)&param_testmode,
    (void*)&param_racemode,
    (void*)&param_pid_d_smoothing,
    (void*)&param_sensor_weight,
    (void*)&param_pid_limit,
    (void*)&param_motor_balance,
    (void*)&param_unstable_smoothing,
};


#endif /* TFFU_PARAMS_H_ */
