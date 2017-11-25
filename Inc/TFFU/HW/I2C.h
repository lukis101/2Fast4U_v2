
#ifndef TFFU_HW_I2C_H_
#define TFFU_HW_I2C_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define I2C_RX_BUFFER_SIZE  256
extern uint8_t i2c_RxBuff[I2C_RX_BUFFER_SIZE];
#define I2CDMA_RX_BUFFER_SIZE  64
extern uint8_t DMA_RX_Buffer[I2CDMA_RX_BUFFER_SIZE];


void I2C_Init(void);
bool I2C_Available(void);
void I2C_Read(uint8_t devAddr, uint8_t amount, uint8_t* pBuff, uint8_t* flag);
void I2C_Send(uint8_t* pdata, uint8_t amount);

#ifdef __cplusplus
}
#endif
#endif /* TFFU_HW_I2C_H_ */
