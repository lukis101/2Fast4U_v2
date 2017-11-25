
#ifndef TFFU_PID_H_
#define TFFU_PID_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

extern float pid_prop;
extern float pid_integ;
extern float pid_deriv;
extern float pid_deriv_avg;
extern float pid_unstable;
extern int16_t pid_output;

extern int16_t lastErr;

void PID_Init();
void PID_Update( int16_t error );

#ifdef __cplusplus
}
#endif
#endif /* TFFU_PID_H_ */
