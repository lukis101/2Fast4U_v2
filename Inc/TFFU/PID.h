
#ifndef TFFU_PID_H_
#define TFFU_PID_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include <stdint.h>
#include "TFFU/Params.h"


class PID
{
public:
	PIDParams_t params;
	float deriv_avg;
	float unstable;
	float output;
	int16_t lastErr;
	PIDParams_t val;
	uint8_t D_HIST_SIZE;  //4
	float d_hist[ 100 ];
	uint8_t d_hist_i;
public:
	void PID_Init();
	float PID_Update( int16_t error );
};


#ifdef __cplusplus
}
#endif
#endif /* TFFU_PID_H_ */
