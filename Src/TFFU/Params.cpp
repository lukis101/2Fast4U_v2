
//#include <i2c.h>
#include <stdint.h>


// PARAMS
int16_t param_maxspeed = 90;
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
float param_unstable_smoothing = 0.04;
