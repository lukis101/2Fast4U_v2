
#ifndef TFFU_COMPASS_H_
#define TFFU_COMPASS_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

 //Magnetometer Registers
 #define AK8963_ADDRESS   0x0C
 #define AK8963_WHO_AM_I  0x00 // should return 0x48
 #define AK8963_INFO      0x01
 #define AK8963_ST1       0x02  // data ready status bit 0
 #define AK8963_XOUT_L	 0x03  // data
 #define AK8963_XOUT_H	 0x04
 #define AK8963_YOUT_L	 0x05
 #define AK8963_YOUT_H	 0x06
 #define AK8963_ZOUT_L	 0x07
 #define AK8963_ZOUT_H	 0x08
 #define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
 #define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
 #define AK8963_ASTC      0x0C  // Self test control
 #define AK8963_I2CDIS    0x0F  // I2C disable
 #define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
 #define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
 #define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define MPU9250_ADDRESS   0x68  // Device address when ADO = 0
#define AK8963_ADDRESS    0x0C  // Address of magnetometer

 enum Mscale {
   MFS_14BITS = 0, // 0.6 mG per LSB
   MFS_16BITS      // 0.15 mG per LSB
 };

class Compass
{
public:
	uint8_t dataReady;
public:
	float direction;

	Compass();
	void Start();
	void Update();
	void Zero();
};

#ifdef __cplusplus
}
#endif
#endif /* TFFU_COMPASS_H_ */
