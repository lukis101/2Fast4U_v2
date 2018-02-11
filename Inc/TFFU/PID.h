
#ifndef TFFU_PID_H_
#define TFFU_PID_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include <stdint.h>
#include "TFFU/Params.h"

#define D_HIST_SIZE 15

class PID
{
protected:
	float lastErr;
public:
	PIDParams_t* params;
	PIDParams_t values;
	float deriv_avg;
	float unstability;
	float output;
	float d_hist[D_HIST_SIZE];
	uint8_t d_hist_i;

public:
	PID(PIDParams_t* params);
	void Init();
	float Update(float error);
};


#ifdef __cplusplus
}
#endif
#endif /* TFFU_PID_H_ */
